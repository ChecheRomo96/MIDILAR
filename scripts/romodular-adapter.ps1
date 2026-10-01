$script:MIDILARRoot = Split-Path -Parent $PSScriptRoot
$script:MIDILARBuildRoot = Join-Path $script:MIDILARRoot "build"
$script:MIDILARDistRoot = Join-Path $script:MIDILARRoot "dist"
$script:MIDILARRoModularScripts = Join-Path `
    $script:MIDILARRoot `
    "tools/RoModularBuild/scripts"

$roModularCommon = Join-Path $script:MIDILARRoModularScripts "common.ps1"
if (-not (Test-Path -LiteralPath $roModularCommon -PathType Leaf)) {
    throw "RoModularBuild is unavailable; initialize tools/RoModularBuild with git submodule update --init --recursive"
}

$env:ROMODULAR_PROJECT_ROOT = $script:MIDILARRoot
$env:ROMODULAR_BUILD_ROOT = $script:MIDILARBuildRoot
$env:ROMODULAR_DIST_ROOT = $script:MIDILARDistRoot
$env:ROMODULAR_PROJECT_LABEL = "MIDILAR"
$env:ROMODULAR_CONFIGURE_COMMAND = "scripts/configure.ps1"
$env:ROMODULAR_DEFAULT_CONFIGURATION = "Debug"
$env:ROMODULAR_DOCUMENTATION_PRESET = "documentation"
$env:ROMODULAR_DOCUMENTATION_CONFIGURATION = "Release"
$env:ROMODULAR_INSTALL_CONFIGURATION = "Release"
$env:ROMODULAR_TESTING_CACHE_ARGUMENT = "-DMIDILAR_TESTING=ON"
$env:ROMODULAR_EXAMPLES_CACHE_ARGUMENT = "-DMIDILAR_EXAMPLES=ON"
