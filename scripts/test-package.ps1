param(
    [Parameter(Mandatory = $true, Position = 0)]
    [string]$Preset,

    [string]$FoundationPrefix = "",
    [int]$Parallel = 0,
    [switch]$Fresh
)

. "$PSScriptRoot/common.ps1"

Assert-MIDILARPreset -Preset $Preset
if ($Preset -notmatch "^(macos|linux|windows)_") {
    throw "Package consumer tests require a runnable desktop preset"
}

# Without an explicit prefix, MIDILAR resolves Foundation itself: sibling export,
# GitHub Release package or sources (see cmake/MIDILARFoundation.cmake).
if (-not $FoundationPrefix -and $env:MIDILAR_FOUNDATION_PREFIX) {
    $FoundationPrefix = $env:MIDILAR_FOUNDATION_PREFIX
}
$exportParameters = @{
    Preset = $Preset
}
if ($FoundationPrefix) {
    $FoundationPrefix = Resolve-MIDILARPath -Path $FoundationPrefix
    $foundationConfig = Join-Path $FoundationPrefix "lib/cmake/Foundation/FoundationConfig.cmake"
    if (-not (Test-Path -LiteralPath $foundationConfig -PathType Leaf)) {
        throw "Foundation package not found at $FoundationPrefix"
    }
    $exportParameters.CMakeArguments = @("-DMIDILAR_FOUNDATION_PREFIX=$FoundationPrefix")
}
if ($Parallel -gt 0) {
    $exportParameters.Parallel = $Parallel
}
if ($Fresh) {
    $exportParameters.Fresh = $true
}
& "$PSScriptRoot/export.ps1" @exportParameters
if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE
}

# The Foundation package MIDILAR used; empty when Foundation was built from
# sources and installed next to MIDILAR.
$cachePath = Join-Path (Get-MIDILARBuildDirectory -Preset $Preset) "CMakeCache.txt"
$resolvedFoundationPrefix = ""
$resolvedLine = Select-String -LiteralPath $cachePath `
    -Pattern "^MIDILAR_FOUNDATION_RESOLVED_PREFIX:INTERNAL=(.*)$" | Select-Object -First 1
if ($resolvedLine) {
    $resolvedFoundationPrefix = $resolvedLine.Matches[0].Groups[1].Value
}
$resolvedMCCPrefix = ""
$resolvedLine = Select-String -LiteralPath $cachePath `
    -Pattern "^MIDILAR_MCC_RESOLVED_PREFIX:INTERNAL=(.*)$" | Select-Object -First 1
if ($resolvedLine) {
    $resolvedMCCPrefix = $resolvedLine.Matches[0].Groups[1].Value
}

$midilarPrefix = Join-Path $script:MIDILARDistRoot $Preset
$consumerSource = Join-Path $script:MIDILARRoot "tests/PackageConsumer"
$consumerBuild = Join-Path $script:MIDILARBuildRoot "package-consumer/$Preset"

$midilarBuildDirectory = Get-MIDILARBuildDirectory -Preset $Preset
$midilarCache = Join-Path $midilarBuildDirectory "CMakeCache.txt"

function Get-MIDILARCacheValue {
    param([Parameter(Mandatory = $true)][string]$Name)

    $match = Select-String `
        -LiteralPath $midilarCache `
        -Pattern "^${Name}:[^=]*=" | `
        Select-Object -First 1

    if (-not $match) {
        return ""
    }

    return ($match.Line -split "=", 2)[1]
}

$generator = Get-MIDILARCacheValue -Name "CMAKE_GENERATOR"
$generatorPlatform = Get-MIDILARCacheValue -Name "CMAKE_GENERATOR_PLATFORM"
$cxxCompiler = Get-MIDILARCacheValue -Name "CMAKE_CXX_COMPILER"
$toolchainFile = Get-MIDILARCacheValue -Name "CMAKE_TOOLCHAIN_FILE"
$osxArchitectures = Get-MIDILARCacheValue -Name "CMAKE_OSX_ARCHITECTURES"
$crossCompiling = Get-MIDILARCacheValue -Name "CMAKE_CROSSCOMPILING"

if ($crossCompiling -eq "TRUE") {
    throw "Package execution requires a native preset"
}
if (-not $generator) {
    throw "Configured preset has no CMake generator"
}

# The package consumer must use the same ABI and compiler family as the
# package. Recreate it so a previous run cannot retain another generator.
if (Test-Path -LiteralPath $consumerBuild) {
    Remove-Item -LiteralPath $consumerBuild -Recurse -Force
}

$configureArguments = @(
    "-S", $consumerSource,
    "-B", $consumerBuild,
    "-G", $generator,
    "-DMIDILAR_DIR=$(Join-Path $midilarPrefix 'lib/cmake/MIDILAR')",
    "-DCMAKE_PREFIX_PATH=$midilarPrefix;$resolvedMCCPrefix;$resolvedFoundationPrefix"
)

if ($generatorPlatform) {
    $configureArguments += @("-A", $generatorPlatform)
}
if ($osxArchitectures) {
    $configureArguments += "-DCMAKE_OSX_ARCHITECTURES=$osxArchitectures"
}
if ($toolchainFile) {
    $configureArguments += "-DCMAKE_TOOLCHAIN_FILE=$toolchainFile"
}
elseif ($cxxCompiler -and $generator -notmatch "^(Visual Studio|Xcode)") {
    $configureArguments += "-DCMAKE_CXX_COMPILER=$cxxCompiler"
}

Invoke-MIDILARCMake -Arguments $configureArguments

$buildArguments = @("--build", $consumerBuild, "--config", "Release")
if ($Parallel -gt 0) {
    $buildArguments += @("--parallel", $Parallel.ToString())
}
Invoke-MIDILARCMake -Arguments $buildArguments

$testArguments = @(
    "--test-dir", $consumerBuild,
    "--output-on-failure",
    "--no-tests=error",
    "--build-config", "Release"
)
if ($Parallel -gt 0) {
    $testArguments += @("--parallel", $Parallel.ToString())
}
& ctest @testArguments
if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE
}

Write-Host "Verified installed MIDILAR package and its transitive MCC and Foundation dependencies"
