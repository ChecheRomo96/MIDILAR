param(
    [string]$Fqbn = "arduino:avr:uno",
    [string]$Foundation = "",
    [string]$DspCore = "",
    [string]$MCC = "",
    [string]$Cpstl = ""
)

$ErrorActionPreference = "Stop"

. "$PSScriptRoot/common.ps1"

# The validated Arduino source-mode board is arduino:avr:uno. Foundation
# defaults to MIDILAR_FOUNDATION_SOURCE or the sibling ../Foundation, and MCC
# to MIDILAR_MCC_SOURCE or the sibling ../MCC.
if (-not (Get-Command arduino-cli -ErrorAction SilentlyContinue)) {
    throw "arduino-cli not found"
}

$root = $script:MIDILARRoot
if (-not $Foundation) {
    $Foundation = if ($env:MIDILAR_FOUNDATION_SOURCE) { $env:MIDILAR_FOUNDATION_SOURCE } else { Join-Path $root "../Foundation" }
}
if (-not (Test-Path -LiteralPath (Join-Path $Foundation "library.properties"))) {
    throw "Foundation Arduino library not found at $Foundation"
}
$Foundation = (Resolve-Path -LiteralPath $Foundation).Path
if (-not $DspCore) {
    $DspCore = if ($env:MIDILAR_DSPCORE_SOURCE) { $env:MIDILAR_DSPCORE_SOURCE } else { Join-Path $root "../DspCore" }
}
if (-not (Test-Path -LiteralPath (Join-Path $DspCore "library.properties"))) {
    throw "DspCore Arduino library not found at $DspCore"
}
$DspCore = (Resolve-Path -LiteralPath $DspCore).Path
# CPSTL defaults to MIDILAR_CPSTL_SOURCE or the sibling ../CPSTL.
if (-not $Cpstl) {
    $Cpstl = if ($env:MIDILAR_CPSTL_SOURCE) { $env:MIDILAR_CPSTL_SOURCE } else { Join-Path $root "../CPSTL" }
}
if (-not (Test-Path -LiteralPath (Join-Path $Cpstl "library.properties"))) {
    throw "CPSTL Arduino library not found at $Cpstl"
}
$Cpstl = (Resolve-Path -LiteralPath $Cpstl).Path
if (-not $MCC) {
    $MCC = if ($env:MIDILAR_MCC_SOURCE) { $env:MIDILAR_MCC_SOURCE } else { Join-Path $root "../MCC" }
}
if (-not (Test-Path -LiteralPath (Join-Path $MCC "library.properties"))) {
    throw "MCC Arduino library not found at $MCC"
}
$MCC = (Resolve-Path -LiteralPath $MCC).Path

$examplesRoot = Join-Path $root "examples/MIDILAR"
$buildRoot = Join-Path $root ("build/arduino/" + ($Fqbn -replace ":", "_"))
if (Test-Path -LiteralPath $buildRoot) {
    Remove-Item -LiteralPath $buildRoot -Recurse -Force
}

# Compile each sketch against the repository, MCC and Foundation as libraries,
# exactly as an Arduino user who installed both would.
$sketches = Get-ChildItem -Path $examplesRoot -Recurse -Filter *.ino |
    Where-Object {
        $relative = $_.DirectoryName.Substring($examplesRoot.Length + 1)
        ($relative -split '[\\/]').Count -eq 2
    } |
    Sort-Object FullName

$count = 0
foreach ($sketch in $sketches) {
    $name = $sketch.DirectoryName.Substring($examplesRoot.Length + 1) -replace '\\', '/'
    $log = Join-Path $buildRoot "$name.log"
    New-Item -ItemType Directory -Force -Path (Split-Path -Parent $log) | Out-Null
    Write-Host "== $name ($Fqbn)"

    & arduino-cli compile `
        --fqbn $Fqbn `
        --library $root `
        --library $MCC `
        --library $Foundation `
        --library $DspCore `
        --library $Cpstl `
        --build-path (Join-Path $buildRoot $name) `
        --warnings default `
        $sketch.DirectoryName *> $log
    $status = $LASTEXITCODE
    Get-Content -LiteralPath $log
    if ($status -ne 0) {
        throw "$name failed to compile"
    }

    # The stock AVR core passes -fpermissive, which demotes real type errors
    # to warnings; any warning in MIDILAR or its examples fails the gate.
    $warnings = Select-String -LiteralPath $log -Pattern "warning:" |
        Where-Object {
            $_.Line.Contains($root) -or $_.Line.Contains($root -replace '\\', '/')
        }
    if ($warnings) {
        throw "$name compiled with MIDILAR warnings"
    }
    $count++
}

if ($count -eq 0) {
    throw "no Arduino sketches found"
}
Write-Host "All $count Arduino sketches compiled for $Fqbn."
