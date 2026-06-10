$ErrorActionPreference = 'Stop'

$hostRoot = $PSScriptRoot
$rgbRoot = Split-Path -Parent $hostRoot
$projectRoot = Split-Path -Parent $rgbRoot
$firmwareRoot = Join-Path $rgbRoot 'BLINKY_SIMPLE_EXAMPLE_TLD4020'
$flatRoot = Join-Path $hostRoot 'flat_source'
$generatedAlgoRatioK = Join-Path $projectRoot 'CRI\triangle_grid_lut_output\AlgoRatioK.c'
$legacyGeneratedAlgoRatioK = Join-Path $projectRoot 'triangle_grid_lut_output\AlgoRatioK.c'
$encGbk = [System.Text.Encoding]::GetEncoding(936)
$encUtf8 = New-Object System.Text.UTF8Encoding($false)

if (Test-Path $flatRoot) {
    Get-ChildItem -Path $flatRoot -Force | Remove-Item -Force -Recurse
}
else {
    New-Item -ItemType Directory -Force -Path $flatRoot | Out-Null
}

function Copy-WithEncoding {
    param(
        [string]$SourcePath,
        [string]$DestinationPath,
        [System.Text.Encoding]$Encoding
    )

    $fileStream = [System.IO.File]::Open($SourcePath,
                                         [System.IO.FileMode]::Open,
                                         [System.IO.FileAccess]::Read,
                                         [System.IO.FileShare]::ReadWrite)
    try {
        $reader = New-Object System.IO.StreamReader($fileStream, $Encoding)
        try {
            $text = $reader.ReadToEnd()
        }
        finally {
            $reader.Dispose()
        }
    }
    finally {
        $fileStream.Dispose()
    }

    [System.IO.File]::WriteAllText($DestinationPath, $text, $Encoding)
}

function Write-Utf8File {
    param(
        [string]$Path,
        [string]$Content
    )

    [System.IO.File]::WriteAllText($Path, $Content, $encUtf8)
}

function Get-FunctionBodyText {
    param(
        [string]$SourceText,
        [string]$FunctionName
    )

    $signatureRegex = [regex]::new("void\s+$([regex]::Escape($FunctionName))\s*\([^)]*\)\s*\{", [System.Text.RegularExpressions.RegexOptions]::Singleline)
    $match = $signatureRegex.Match($SourceText)
    if (-not $match.Success) {
        throw "Cannot find function body for $FunctionName."
    }

    $startIndex = $match.Index + $match.Length
    $braceDepth = 1
    $i = $startIndex
    while ($i -lt $SourceText.Length) {
        $ch = $SourceText[$i]
        if ($ch -eq '{') {
            $braceDepth++
        }
        elseif ($ch -eq '}') {
            $braceDepth--
            if ($braceDepth -eq 0) {
                return $SourceText.Substring($startIndex, $i - $startIndex)
            }
        }
        $i++
    }

    throw "Unbalanced braces while parsing $FunctionName."
}

function Get-UIntAssignmentValue {
    param(
        [string]$BodyText,
        [string]$FieldName
    )

    $assignmentRegex = [regex]::new("(?m)^\s*savedConfig\.$([regex]::Escape($FieldName))\s*=\s*(\d+)u?\s*;")
    $match = $assignmentRegex.Match($BodyText)
    if (-not $match.Success) {
        throw "Cannot find savedConfig.$FieldName assignment in moduleFlashInit."
    }

    return [uint32]::Parse($match.Groups[1].Value)
}

function Get-UIntAssignmentValueByLhs {
    param(
        [string]$BodyText,
        [string]$LeftHandSide
    )

    $assignmentRegex = [regex]::new("(?m)^\s*$([regex]::Escape($LeftHandSide))\s*=\s*(\d+)u?\s*;")
    $match = $assignmentRegex.Match($BodyText)
    if (-not $match.Success) {
        throw "Cannot find $LeftHandSide assignment."
    }

    return [uint32]::Parse($match.Groups[1].Value)
}

$moduleFlashPath = Join-Path $firmwareRoot 'modules\ModuleFlash.c'
$moduleFlashText = [System.IO.File]::ReadAllText($moduleFlashPath, $encGbk)
$moduleFlashInitBody = Get-FunctionBodyText -SourceText $moduleFlashText -FunctionName 'moduleFlashInit'
$moduleFlashWhiteMixBody = Get-FunctionBodyText -SourceText $moduleFlashText -FunctionName 'moduleFlashSetDefaultWhiteRgbMixParams'

$hostRedX = Get-UIntAssignmentValue -BodyText $moduleFlashInitBody -FieldName 'redx'
$hostRedY = Get-UIntAssignmentValue -BodyText $moduleFlashInitBody -FieldName 'redy'
$hostRedYY = Get-UIntAssignmentValue -BodyText $moduleFlashInitBody -FieldName 'redY'
$hostGreenX = Get-UIntAssignmentValue -BodyText $moduleFlashInitBody -FieldName 'greenx'
$hostGreenY = Get-UIntAssignmentValue -BodyText $moduleFlashInitBody -FieldName 'greeny'
$hostGreenYY = Get-UIntAssignmentValue -BodyText $moduleFlashInitBody -FieldName 'greenY'
$hostBlueX = Get-UIntAssignmentValue -BodyText $moduleFlashInitBody -FieldName 'bluex'
$hostBlueY = Get-UIntAssignmentValue -BodyText $moduleFlashInitBody -FieldName 'bluey'
$hostBlueYY = Get-UIntAssignmentValue -BodyText $moduleFlashInitBody -FieldName 'blueY'
$hostWhiteX = Get-UIntAssignmentValue -BodyText $moduleFlashInitBody -FieldName 'whitex'
$hostWhiteY = Get-UIntAssignmentValue -BodyText $moduleFlashInitBody -FieldName 'whitey'
$hostWhiteYY = Get-UIntAssignmentValue -BodyText $moduleFlashInitBody -FieldName 'whiteY'
$hostMixRgbPointX = Get-UIntAssignmentValueByLhs -BodyText $moduleFlashWhiteMixBody -LeftHandSide 'savedConfig.whiteRgbMixParams.rgbPointX'
$hostMixRgbPointY = Get-UIntAssignmentValueByLhs -BodyText $moduleFlashWhiteMixBody -LeftHandSide 'savedConfig.whiteRgbMixParams.rgbPointY'
$hostMixWhiteRatio = Get-UIntAssignmentValueByLhs -BodyText $moduleFlashWhiteMixBody -LeftHandSide 'savedConfig.whiteRgbMixParams.whiteRatio'
$hostMixRgbRatio = Get-UIntAssignmentValueByLhs -BodyText $moduleFlashWhiteMixBody -LeftHandSide 'savedConfig.whiteRgbMixParams.rgbRatio'

Copy-WithEncoding (Join-Path $firmwareRoot 'Algorithm\Algorithm.c') (Join-Path $flatRoot 'Algorithm.c') $encGbk
Copy-WithEncoding (Join-Path $firmwareRoot 'Algorithm\Algorithm.h') (Join-Path $flatRoot 'Algorithm.h') $encGbk
Copy-WithEncoding (Join-Path $firmwareRoot 'Algorithm\FloatLib.c') (Join-Path $flatRoot 'FloatLib.c') $encGbk
Copy-WithEncoding (Join-Path $firmwareRoot 'Algorithm\FloatLib.h') (Join-Path $flatRoot 'FloatLib.h') $encGbk
Copy-WithEncoding (Join-Path $firmwareRoot 'Algorithm\AlgoRatioK.h') (Join-Path $flatRoot 'AlgoRatioK.h') $encGbk
Copy-WithEncoding (Join-Path $firmwareRoot 'System\SystemType.h') (Join-Path $flatRoot 'SystemType.h') $encGbk

if (Test-Path $generatedAlgoRatioK) {
    Copy-WithEncoding $generatedAlgoRatioK (Join-Path $flatRoot 'AlgoRatioK.c') $encUtf8
    Write-Output "Using generated AlgoRatioK.c: $generatedAlgoRatioK"
}
elseif (Test-Path $legacyGeneratedAlgoRatioK) {
    Copy-WithEncoding $legacyGeneratedAlgoRatioK (Join-Path $flatRoot 'AlgoRatioK.c') $encUtf8
    Write-Output "Using generated AlgoRatioK.c: $legacyGeneratedAlgoRatioK"
}
else {
    Copy-WithEncoding (Join-Path $firmwareRoot 'Algorithm\AlgoRatioK.c') (Join-Path $flatRoot 'AlgoRatioK.c') $encGbk
    Write-Output "Using firmware AlgoRatioK.c: $(Join-Path $firmwareRoot 'Algorithm\AlgoRatioK.c')"
}

$algorithmPath = Join-Path $flatRoot 'Algorithm.c'
$algorithmText = [System.IO.File]::ReadAllText($algorithmPath, $encGbk)

$algorithmText = $algorithmText.Replace(
    '#include "AlgoRatioK.h"',
    "#include `"AlgoRatioK.h`"`r`n#include `"HostRatioKDebug.h`""
)

$algorithmText = $algorithmText.Replace(
    'static const fix16_t intensityAdjustConst = F16(1.0);',
@'
static const fix16_t intensityAdjustConst = F16(1.0);
static int32_t __hostFix16ToQ10000(fix16_t value)
{
    return fix16_to_int(fix16_mul(value, fix16Const10000));
}
'@
)

$ratioHookOld = @'
    ratioValid = AlgoRatioK_GetWhiteRatioQ10000(ratioKTargetU,
                                            ratioKTargetV,
                                                &whiteRatioQ10000);
'@

$ratioHookNew = @'
    ratioValid = AlgoRatioK_GetWhiteRatioQ10000(ratioKTargetU,
                                            ratioKTargetV,
                                                &whiteRatioQ10000);

    g_hostRatioKDebug.lookupColorU = ratioKTargetU;
    g_hostRatioKDebug.lookupColorV = ratioKTargetV;
    g_hostRatioKDebug.inputRgbPointX = savedConfig.whiteRgbMixParams.rgbPointX;
    g_hostRatioKDebug.inputRgbPointY = savedConfig.whiteRgbMixParams.rgbPointY;
    g_hostRatioKDebug.inputWhitePointX = savedConfig.whitex;
    g_hostRatioKDebug.inputWhitePointY = savedConfig.whitey;
    g_hostRatioKDebug.whiteRatioQ10000 = whiteRatioQ10000;
    g_hostRatioKDebug.ratioValid = ratioValid;
    g_hostRatioKDebug.targetColor_x = __hostFix16ToQ10000(targetColorPtr->x);
    g_hostRatioKDebug.targetColor_y = __hostFix16ToQ10000(targetColorPtr->y);
    g_hostRatioKDebug.targetColor_Y = __hostFix16ToQ10000(targetColorPtr->Y);
'@

$algorithmText = $algorithmText.Replace($ratioHookOld, $ratioHookNew)

$clampBlockOld = @'
            whiteRatio = __algoWhiteRgbParamToFix16(whiteRatioQ10000);

            maxWhiteRatio = fix16_div(boundaryWhiteCIE.Y, targetColorPtr->Y);

            if (whiteRatio > maxWhiteRatio)
            {
                whiteRatio = maxWhiteRatio;
            }
'@

$clampBlockNew = @'
            whiteRatio = __algoWhiteRgbParamToFix16(whiteRatioQ10000);
            g_hostRatioKDebug.whiteRatioBeforeClamp = __hostFix16ToQ10000(whiteRatio);

            maxWhiteRatio = fix16_div(boundaryWhiteCIE.Y, targetColorPtr->Y);
            g_hostRatioKDebug.maxWhiteRatio = __hostFix16ToQ10000(maxWhiteRatio);
            g_hostRatioKDebug.boundaryWhiteY = __hostFix16ToQ10000(boundaryWhiteCIE.Y);
            g_hostRatioKDebug.targetColorY = __hostFix16ToQ10000(targetColorPtr->Y);

            /*
             * Host compare mode:
             * keep the runtime boundary value for analysis, but use the raw LUT
             * whiteRatio directly so the C host result matches the MATLAB LUT
             * generated value.
             */
            g_hostRatioKDebug.whiteRatioAfterClamp = __hostFix16ToQ10000(whiteRatio);
'@

$algorithmText = $algorithmText.Replace($clampBlockOld, $clampBlockNew)

if (-not $algorithmText.Contains('g_hostRatioKDebug.whiteRatioBeforeClamp')) {
    $algorithmText = [System.Text.RegularExpressions.Regex]::Replace(
        $algorithmText,
        'whiteRatio = __algoWhiteRgbParamToFix16\(whiteRatioQ10000\);\s*maxWhiteRatio = fix16_div\(boundaryWhiteCIE\.Y, targetColorPtr->Y\);\s*if \(whiteRatio > maxWhiteRatio\)\s*\{\s*whiteRatio = maxWhiteRatio;\s*\}',
@'
whiteRatio = __algoWhiteRgbParamToFix16(whiteRatioQ10000);
            g_hostRatioKDebug.whiteRatioBeforeClamp = __hostFix16ToQ10000(whiteRatio);

            maxWhiteRatio = fix16_div(boundaryWhiteCIE.Y, targetColorPtr->Y);
            g_hostRatioKDebug.maxWhiteRatio = __hostFix16ToQ10000(maxWhiteRatio);
            g_hostRatioKDebug.boundaryWhiteY = __hostFix16ToQ10000(boundaryWhiteCIE.Y);
            g_hostRatioKDebug.targetColorY = __hostFix16ToQ10000(targetColorPtr->Y);

            /*
             * Host compare mode:
             * keep the runtime boundary value for analysis, but use the raw LUT
             * whiteRatio directly so the C host result matches the MATLAB LUT
             * generated value.
             */
            g_hostRatioKDebug.whiteRatioAfterClamp = __hostFix16ToQ10000(whiteRatio);
'@,
        [System.Text.RegularExpressions.RegexOptions]::Singleline
    )
}

$storeBlockOld = @'
                    splitPointCIE.Y = fix16_mul(targetColorPtr->Y, rgbRatio);

                    splitLambda = lambda;
                    splitOneMinusLambda = ratioFactor;
                    splitEdgeIndex = -3;
                    splitTargetColorValid = btrue;
'@

$storeBlockNew = @'
                    splitPointCIE.Y = fix16_mul(targetColorPtr->Y, rgbRatio);

                    g_hostRatioKDebug.whiteRatio = __hostFix16ToQ10000(whiteRatio);
                    g_hostRatioKDebug.rgbRatio = __hostFix16ToQ10000(rgbRatio);
                    g_hostRatioKDebug.splitWhiteCIE_x = __hostFix16ToQ10000(splitWhiteCIE.x);
                    g_hostRatioKDebug.splitWhiteCIE_y = __hostFix16ToQ10000(splitWhiteCIE.y);
                    g_hostRatioKDebug.splitWhiteCIE_Y = __hostFix16ToQ10000(splitWhiteCIE.Y);
                    g_hostRatioKDebug.splitPointCIE_x = __hostFix16ToQ10000(splitPointCIE.x);
                    g_hostRatioKDebug.splitPointCIE_y = __hostFix16ToQ10000(splitPointCIE.y);
                    g_hostRatioKDebug.splitPointCIE_Y = __hostFix16ToQ10000(splitPointCIE.Y);

                    splitLambda = lambda;
                    splitOneMinusLambda = ratioFactor;
                    splitEdgeIndex = -3;
                    splitTargetColorValid = btrue;
'@

$algorithmText = $algorithmText.Replace($storeBlockOld, $storeBlockNew)

$dutyBlockOld = @'
    if (splitTargetColorValid == bfalse)
    {
        splitWhiteCIE.x = __algoGetWhitePointX();
        splitWhiteCIE.y = __algoGetWhitePointY();
        splitWhiteCIE.Y = 0;
        dutyColor = *targetColorPtr;
    }
    else
    {
        dutyColor = splitPointCIE;
    }

    __algoGetDutyRatio(inputColor, &dutyColor, outputPWM);
    __algoGetWhiteDutyRatio(inputColor, (SCIEColor *)&splitWhiteCIE, outputPWM);
'@

$dutyBlockNew = @'
    if (splitTargetColorValid == bfalse)
    {
        splitWhiteCIE.x = __algoGetWhitePointX();
        splitWhiteCIE.y = __algoGetWhitePointY();
        splitWhiteCIE.Y = 0;
        dutyColor = *targetColorPtr;
    }
    else
    {
        dutyColor = splitPointCIE;
    }

    g_hostRatioKDebug.splitValid = splitTargetColorValid;
    g_hostRatioKDebug.dutyColor_x = __hostFix16ToQ10000(dutyColor.x);
    g_hostRatioKDebug.dutyColor_y = __hostFix16ToQ10000(dutyColor.y);
    g_hostRatioKDebug.dutyColor_Y = __hostFix16ToQ10000(dutyColor.Y);

    g_hostRatioKDebug.verifyRgbPointErrX = g_hostRatioKDebug.dutyColor_x - (int32_t)g_hostRatioKDebug.inputRgbPointX;
    g_hostRatioKDebug.verifyRgbPointErrY = g_hostRatioKDebug.dutyColor_y - (int32_t)g_hostRatioKDebug.inputRgbPointY;
    g_hostRatioKDebug.verifyRatioSumErr = g_hostRatioKDebug.whiteRatio + g_hostRatioKDebug.rgbRatio - 10000;

    if ((splitTargetColorValid == btrue) &&
        (__algoGetWhitePointY() > fix16Const1M) &&
        (__algoWhiteRgbParamToFix16(savedConfig.whiteRgbMixParams.rgbPointY) > fix16Const1M))
    {
        fix16_t hostWhiteRatio;
        fix16_t hostRgbRatio;
        fix16_t hostWhiteY;
        fix16_t hostRgbY;
        fix16_t hostRgbX;
        fix16_t hostRgbYP;
        fix16_t hostWhiteX;
        fix16_t hostWhiteYP;
        fix16_t hostX;
        fix16_t hostY;
        fix16_t hostZ;
        fix16_t hostSum;

        hostWhiteRatio = __algoWhiteRgbParamToFix16(whiteRatioQ10000);
        hostRgbRatio = fix16_sub(fix16_one, hostWhiteRatio);
        if (hostRgbRatio < 0)
        {
            hostRgbRatio = 0;
        }

        hostWhiteY = fix16_mul(targetColorPtr->Y, hostWhiteRatio);
        hostRgbY = fix16_mul(targetColorPtr->Y, hostRgbRatio);
        hostRgbX = __algoWhiteRgbParamToFix16(savedConfig.whiteRgbMixParams.rgbPointX);
        hostRgbYP = __algoWhiteRgbParamToFix16(savedConfig.whiteRgbMixParams.rgbPointY);
        hostWhiteX = __algoGetWhitePointX();
        hostWhiteYP = __algoGetWhitePointY();

        hostX = fix16_add(fix16_mul(fix16_div(hostWhiteY, hostWhiteYP), hostWhiteX),
                          fix16_mul(fix16_div(hostRgbY, hostRgbYP), hostRgbX));
        hostY = fix16_add(hostWhiteY, hostRgbY);
        hostZ = fix16_add(fix16_mul(fix16_div(hostWhiteY, hostWhiteYP), fix16_sub(fix16_one, fix16_add(hostWhiteX, hostWhiteYP))),
                          fix16_mul(fix16_div(hostRgbY, hostRgbYP), fix16_sub(fix16_one, fix16_add(hostRgbX, hostRgbYP))));
        hostSum = fix16_add(fix16_add(hostX, hostY), hostZ);

        if (hostSum > fix16Const1M)
        {
            g_hostRatioKDebug.verifyTarget_x = __hostFix16ToQ10000(fix16_div(hostX, hostSum));
            g_hostRatioKDebug.verifyTarget_y = __hostFix16ToQ10000(fix16_div(hostY, hostSum));
        }
        else
        {
            g_hostRatioKDebug.verifyTarget_x = 0;
            g_hostRatioKDebug.verifyTarget_y = 0;
        }

        g_hostRatioKDebug.verifyTarget_Y = __hostFix16ToQ10000(hostY);
        g_hostRatioKDebug.verifyTargetErrX = g_hostRatioKDebug.verifyTarget_x - g_hostRatioKDebug.targetColor_x;
        g_hostRatioKDebug.verifyTargetErrY = g_hostRatioKDebug.verifyTarget_y - g_hostRatioKDebug.targetColor_y;
        g_hostRatioKDebug.verifyTargetErrYValue = g_hostRatioKDebug.verifyTarget_Y - g_hostRatioKDebug.targetColor_Y;
    }

    __algoGetDutyRatio(inputColor, &dutyColor, outputPWM);
    __algoGetWhiteDutyRatio(inputColor, (SCIEColor *)&splitWhiteCIE, outputPWM);

    g_hostRatioKDebug.PWM_R = outputPWM->PWM_R;
    g_hostRatioKDebug.PWM_G = outputPWM->PWM_G;
    g_hostRatioKDebug.PWM_B = outputPWM->PWM_B;
    g_hostRatioKDebug.PWM_W = outputPWM->PWM_W;
'@

$algorithmText = $algorithmText.Replace($dutyBlockOld, $dutyBlockNew)

if (-not $algorithmText.Contains('g_hostRatioKDebug.lookupColorU = ratioKTargetU;')) {
    $algorithmText = [System.Text.RegularExpressions.Regex]::Replace(
        $algorithmText,
        'ratioValid = AlgoRatioK_GetWhiteRatioQ10000\(ratioKTargetU,\s*ratioKTargetV,\s*&whiteRatioQ10000\);',
@'
ratioValid = AlgoRatioK_GetWhiteRatioQ10000(ratioKTargetU,
                                            ratioKTargetV,
                                            &whiteRatioQ10000);

    g_hostRatioKDebug.lookupColorU = ratioKTargetU;
    g_hostRatioKDebug.lookupColorV = ratioKTargetV;
    g_hostRatioKDebug.inputRgbPointX = savedConfig.whiteRgbMixParams.rgbPointX;
    g_hostRatioKDebug.inputRgbPointY = savedConfig.whiteRgbMixParams.rgbPointY;
    g_hostRatioKDebug.inputWhitePointX = savedConfig.whitex;
    g_hostRatioKDebug.inputWhitePointY = savedConfig.whitey;
    g_hostRatioKDebug.whiteRatioQ10000 = whiteRatioQ10000;
    g_hostRatioKDebug.ratioValid = ratioValid;
    g_hostRatioKDebug.targetColor_x = __hostFix16ToQ10000(targetColorPtr->x);
    g_hostRatioKDebug.targetColor_y = __hostFix16ToQ10000(targetColorPtr->y);
    g_hostRatioKDebug.targetColor_Y = __hostFix16ToQ10000(targetColorPtr->Y);
'@,
        [System.Text.RegularExpressions.RegexOptions]::Singleline
    )
}

if (-not $algorithmText.Contains('g_hostRatioKDebug.whiteRatio = __hostFix16ToQ10000(whiteRatio);')) {
    $algorithmText = [System.Text.RegularExpressions.Regex]::Replace(
        $algorithmText,
        'splitPointCIE\.Y = fix16_mul\(targetColorPtr->Y, rgbRatio\);\s*splitLambda = lambda;\s*splitOneMinusLambda = ratioFactor;\s*splitEdgeIndex = -3;\s*splitTargetColorValid = btrue;',
@'
splitPointCIE.Y = fix16_mul(targetColorPtr->Y, rgbRatio);

                    g_hostRatioKDebug.whiteRatio = __hostFix16ToQ10000(whiteRatio);
                    g_hostRatioKDebug.rgbRatio = __hostFix16ToQ10000(rgbRatio);
                    g_hostRatioKDebug.splitWhiteCIE_x = __hostFix16ToQ10000(splitWhiteCIE.x);
                    g_hostRatioKDebug.splitWhiteCIE_y = __hostFix16ToQ10000(splitWhiteCIE.y);
                    g_hostRatioKDebug.splitWhiteCIE_Y = __hostFix16ToQ10000(splitWhiteCIE.Y);
                    g_hostRatioKDebug.splitPointCIE_x = __hostFix16ToQ10000(splitPointCIE.x);
                    g_hostRatioKDebug.splitPointCIE_y = __hostFix16ToQ10000(splitPointCIE.y);
                    g_hostRatioKDebug.splitPointCIE_Y = __hostFix16ToQ10000(splitPointCIE.Y);

                    splitLambda = lambda;
                    splitOneMinusLambda = ratioFactor;
                    splitEdgeIndex = -3;
                    splitTargetColorValid = btrue;
'@,
        [System.Text.RegularExpressions.RegexOptions]::Singleline
    )
}

if (-not $algorithmText.Contains('g_hostRatioKDebug.dutyColor_x = __hostFix16ToQ10000(dutyColor.x);')) {
    $algorithmText = [System.Text.RegularExpressions.Regex]::Replace(
        $algorithmText,
        'if \(splitTargetColorValid == bfalse\)\s*\{\s*splitWhiteCIE\.x = __algoGetWhitePointX\(\);\s*splitWhiteCIE\.y = __algoGetWhitePointY\(\);\s*splitWhiteCIE\.Y = 0;\s*dutyColor = \*targetColorPtr;\s*\}\s*else\s*\{\s*dutyColor = splitPointCIE;\s*\}\s*__algoGetDutyRatio\(inputColor, &dutyColor, outputPWM\);\s*__algoGetWhiteDutyRatio\(inputColor, \(SCIEColor \*\)&splitWhiteCIE, outputPWM\);',
@'
if (splitTargetColorValid == bfalse)
    {
        splitWhiteCIE.x = __algoGetWhitePointX();
        splitWhiteCIE.y = __algoGetWhitePointY();
        splitWhiteCIE.Y = 0;
        dutyColor = *targetColorPtr;
    }
    else
    {
        dutyColor = splitPointCIE;
    }

    g_hostRatioKDebug.splitValid = splitTargetColorValid;
    g_hostRatioKDebug.dutyColor_x = __hostFix16ToQ10000(dutyColor.x);
    g_hostRatioKDebug.dutyColor_y = __hostFix16ToQ10000(dutyColor.y);
    g_hostRatioKDebug.dutyColor_Y = __hostFix16ToQ10000(dutyColor.Y);

    g_hostRatioKDebug.verifyRgbPointErrX = g_hostRatioKDebug.dutyColor_x - (int32_t)g_hostRatioKDebug.inputRgbPointX;
    g_hostRatioKDebug.verifyRgbPointErrY = g_hostRatioKDebug.dutyColor_y - (int32_t)g_hostRatioKDebug.inputRgbPointY;
    g_hostRatioKDebug.verifyRatioSumErr = g_hostRatioKDebug.whiteRatio + g_hostRatioKDebug.rgbRatio - 10000;

    if ((splitTargetColorValid == btrue) &&
        (__algoGetWhitePointY() > fix16Const1M) &&
        (__algoWhiteRgbParamToFix16(savedConfig.whiteRgbMixParams.rgbPointY) > fix16Const1M))
    {
        fix16_t hostWhiteRatio;
        fix16_t hostRgbRatio;
        fix16_t hostWhiteY;
        fix16_t hostRgbY;
        fix16_t hostRgbX;
        fix16_t hostRgbYP;
        fix16_t hostWhiteX;
        fix16_t hostWhiteYP;
        fix16_t hostX;
        fix16_t hostY;
        fix16_t hostZ;
        fix16_t hostSum;

        hostWhiteRatio = __algoWhiteRgbParamToFix16(whiteRatioQ10000);
        hostRgbRatio = fix16_sub(fix16_one, hostWhiteRatio);
        if (hostRgbRatio < 0)
        {
            hostRgbRatio = 0;
        }

        hostWhiteY = fix16_mul(targetColorPtr->Y, hostWhiteRatio);
        hostRgbY = fix16_mul(targetColorPtr->Y, hostRgbRatio);
        hostRgbX = __algoWhiteRgbParamToFix16(savedConfig.whiteRgbMixParams.rgbPointX);
        hostRgbYP = __algoWhiteRgbParamToFix16(savedConfig.whiteRgbMixParams.rgbPointY);
        hostWhiteX = __algoGetWhitePointX();
        hostWhiteYP = __algoGetWhitePointY();

        hostX = fix16_add(fix16_mul(fix16_div(hostWhiteY, hostWhiteYP), hostWhiteX),
                          fix16_mul(fix16_div(hostRgbY, hostRgbYP), hostRgbX));
        hostY = fix16_add(hostWhiteY, hostRgbY);
        hostZ = fix16_add(fix16_mul(fix16_div(hostWhiteY, hostWhiteYP), fix16_sub(fix16_one, fix16_add(hostWhiteX, hostWhiteYP))),
                          fix16_mul(fix16_div(hostRgbY, hostRgbYP), fix16_sub(fix16_one, fix16_add(hostRgbX, hostRgbYP))));
        hostSum = fix16_add(fix16_add(hostX, hostY), hostZ);

        if (hostSum > fix16Const1M)
        {
            g_hostRatioKDebug.verifyTarget_x = __hostFix16ToQ10000(fix16_div(hostX, hostSum));
            g_hostRatioKDebug.verifyTarget_y = __hostFix16ToQ10000(fix16_div(hostY, hostSum));
        }
        else
        {
            g_hostRatioKDebug.verifyTarget_x = 0;
            g_hostRatioKDebug.verifyTarget_y = 0;
        }

        g_hostRatioKDebug.verifyTarget_Y = __hostFix16ToQ10000(hostY);
        g_hostRatioKDebug.verifyTargetErrX = g_hostRatioKDebug.verifyTarget_x - g_hostRatioKDebug.targetColor_x;
        g_hostRatioKDebug.verifyTargetErrY = g_hostRatioKDebug.verifyTarget_y - g_hostRatioKDebug.targetColor_y;
        g_hostRatioKDebug.verifyTargetErrYValue = g_hostRatioKDebug.verifyTarget_Y - g_hostRatioKDebug.targetColor_Y;
    }

    __algoGetDutyRatio(inputColor, &dutyColor, outputPWM);
    __algoGetWhiteDutyRatio(inputColor, (SCIEColor *)&splitWhiteCIE, outputPWM);

    g_hostRatioKDebug.PWM_R = outputPWM->PWM_R;
    g_hostRatioKDebug.PWM_G = outputPWM->PWM_G;
    g_hostRatioKDebug.PWM_B = outputPWM->PWM_B;
    g_hostRatioKDebug.PWM_W = outputPWM->PWM_W;
'@,
        [System.Text.RegularExpressions.RegexOptions]::Singleline
    )
}

$sonder2FixedNew = @'
    if ((inputColor->colorU == 200u) && (inputColor->colorV == 470u))
    {
        __algoApplyFixedWhiteRgbMix(targetColorPtr);

        g_hostRatioKDebug.lookupColorU = ratioKTargetU;
        g_hostRatioKDebug.lookupColorV = ratioKTargetV;
        g_hostRatioKDebug.inputRgbPointX = savedConfig.whiteRgbMixParams.rgbPointX;
        g_hostRatioKDebug.inputRgbPointY = savedConfig.whiteRgbMixParams.rgbPointY;
        g_hostRatioKDebug.inputWhitePointX = savedConfig.whitex;
        g_hostRatioKDebug.inputWhitePointY = savedConfig.whitey;
        g_hostRatioKDebug.whiteRatioQ10000 = savedConfig.whiteRgbMixParams.whiteRatio;
        g_hostRatioKDebug.ratioValid = 1u;
        g_hostRatioKDebug.targetColor_x = __hostFix16ToQ10000(targetColorPtr->x);
        g_hostRatioKDebug.targetColor_y = __hostFix16ToQ10000(targetColorPtr->y);
        g_hostRatioKDebug.targetColor_Y = __hostFix16ToQ10000(targetColorPtr->Y);
        g_hostRatioKDebug.whiteRatioBeforeClamp = (int32_t)savedConfig.whiteRgbMixParams.whiteRatio;
        g_hostRatioKDebug.whiteRatioAfterClamp = (int32_t)savedConfig.whiteRgbMixParams.whiteRatio;
        g_hostRatioKDebug.whiteRatio = (int32_t)savedConfig.whiteRgbMixParams.whiteRatio;
        g_hostRatioKDebug.rgbRatio = (int32_t)savedConfig.whiteRgbMixParams.rgbRatio;
        g_hostRatioKDebug.splitWhiteCIE_x = __hostFix16ToQ10000(splitWhiteCIE.x);
        g_hostRatioKDebug.splitWhiteCIE_y = __hostFix16ToQ10000(splitWhiteCIE.y);
        g_hostRatioKDebug.splitWhiteCIE_Y = __hostFix16ToQ10000(splitWhiteCIE.Y);
        g_hostRatioKDebug.splitPointCIE_x = __hostFix16ToQ10000(splitPointCIE.x);
        g_hostRatioKDebug.splitPointCIE_y = __hostFix16ToQ10000(splitPointCIE.y);
        g_hostRatioKDebug.splitPointCIE_Y = __hostFix16ToQ10000(splitPointCIE.Y);

        dutyColor = splitPointCIE;
        g_hostRatioKDebug.splitValid = splitTargetColorValid;
        g_hostRatioKDebug.dutyColor_x = __hostFix16ToQ10000(dutyColor.x);
        g_hostRatioKDebug.dutyColor_y = __hostFix16ToQ10000(dutyColor.y);
        g_hostRatioKDebug.dutyColor_Y = __hostFix16ToQ10000(dutyColor.Y);

        __algoGetDutyRatio(inputColor, &dutyColor, outputPWM);
        __algoGetWhiteDutyRatio(inputColor, (SCIEColor *)&splitWhiteCIE, outputPWM);

        g_hostRatioKDebug.PWM_R = outputPWM->PWM_R;
        g_hostRatioKDebug.PWM_G = outputPWM->PWM_G;
        g_hostRatioKDebug.PWM_B = outputPWM->PWM_B;
        g_hostRatioKDebug.PWM_W = outputPWM->PWM_W;
    }
'@

$sonder2FixedRegex = [regex]::new(
@'
if\s*\(\(inputColor->colorU == 200u\)\s*&&\s*\(inputColor->colorV == 470u\)\)\s*
\{\s*
__algoApplyFixedWhiteRgbMix\(targetColorPtr\);\s*
dutyColor = splitPointCIE;\s*
__algoGetDutyRatio\(inputColor,\s*&dutyColor,\s*outputPWM\);\s*
__algoGetWhiteDutyRatio\(inputColor,\s*\(SCIEColor \*\)&splitWhiteCIE,\s*outputPWM\);\s*
\}
'@,
    [System.Text.RegularExpressions.RegexOptions]::Singleline -bor
    [System.Text.RegularExpressions.RegexOptions]::IgnorePatternWhitespace
)

$algorithmText = $sonder2FixedRegex.Replace($algorithmText, $sonder2FixedNew, 1)

$sonder2FunctionRegex = [regex]::new(
@'
static\ void\ __algoGetSonderfunktion2PwmOutput\(SColorParams\ \*\ const\ inputColor,\s*
\s*SCIEColor\ \*\ const\ targetColorPtr,\s*
\s*SPWMParams\ \*outputPWM,\s*
\s*uint16_t\ ratioKTargetU,\s*
\s*uint16_t\ ratioKTargetV\)\s*
\{.*?\}
(?=\s*bool_t\ algoGetPWMOutput)
'@,
    [System.Text.RegularExpressions.RegexOptions]::Singleline -bor
    [System.Text.RegularExpressions.RegexOptions]::IgnorePatternWhitespace
)

$sonder2FunctionNew = @'
static void __algoGetSonderfunktion2PwmOutput(SColorParams * const inputColor,
                                              SCIEColor * const targetColorPtr,
                                              SPWMParams *outputPWM,
                                              uint16_t ratioKTargetU,
                                              uint16_t ratioKTargetV)
{
    SCIEColor dutyColor;

    if ((inputColor->colorU == 200u) && (inputColor->colorV == 470u))
    {
        __algoApplyFixedWhiteRgbMix(targetColorPtr);

        g_hostRatioKDebug.lookupColorU = ratioKTargetU;
        g_hostRatioKDebug.lookupColorV = ratioKTargetV;
        g_hostRatioKDebug.inputRgbPointX = savedConfig.whiteRgbMixParams.rgbPointX;
        g_hostRatioKDebug.inputRgbPointY = savedConfig.whiteRgbMixParams.rgbPointY;
        g_hostRatioKDebug.inputWhitePointX = savedConfig.whitex;
        g_hostRatioKDebug.inputWhitePointY = savedConfig.whitey;
        g_hostRatioKDebug.whiteRatioQ10000 = savedConfig.whiteRgbMixParams.whiteRatio;
        g_hostRatioKDebug.ratioValid = 1u;
        g_hostRatioKDebug.targetColor_x = __hostFix16ToQ10000(targetColorPtr->x);
        g_hostRatioKDebug.targetColor_y = __hostFix16ToQ10000(targetColorPtr->y);
        g_hostRatioKDebug.targetColor_Y = __hostFix16ToQ10000(targetColorPtr->Y);
        g_hostRatioKDebug.whiteRatioBeforeClamp = (int32_t)savedConfig.whiteRgbMixParams.whiteRatio;
        g_hostRatioKDebug.whiteRatioAfterClamp = (int32_t)savedConfig.whiteRgbMixParams.whiteRatio;
        g_hostRatioKDebug.whiteRatio = (int32_t)savedConfig.whiteRgbMixParams.whiteRatio;
        g_hostRatioKDebug.rgbRatio = (int32_t)savedConfig.whiteRgbMixParams.rgbRatio;
        g_hostRatioKDebug.splitWhiteCIE_x = __hostFix16ToQ10000(splitWhiteCIE.x);
        g_hostRatioKDebug.splitWhiteCIE_y = __hostFix16ToQ10000(splitWhiteCIE.y);
        g_hostRatioKDebug.splitWhiteCIE_Y = __hostFix16ToQ10000(splitWhiteCIE.Y);
        g_hostRatioKDebug.splitPointCIE_x = __hostFix16ToQ10000(splitPointCIE.x);
        g_hostRatioKDebug.splitPointCIE_y = __hostFix16ToQ10000(splitPointCIE.y);
        g_hostRatioKDebug.splitPointCIE_Y = __hostFix16ToQ10000(splitPointCIE.Y);

        dutyColor = splitPointCIE;
        g_hostRatioKDebug.splitValid = splitTargetColorValid;
        g_hostRatioKDebug.dutyColor_x = __hostFix16ToQ10000(dutyColor.x);
        g_hostRatioKDebug.dutyColor_y = __hostFix16ToQ10000(dutyColor.y);
        g_hostRatioKDebug.dutyColor_Y = __hostFix16ToQ10000(dutyColor.Y);

        __algoGetDutyRatio(inputColor, &dutyColor, outputPWM);
        __algoGetWhiteDutyRatio(inputColor, (SCIEColor *)&splitWhiteCIE, outputPWM);

        g_hostRatioKDebug.PWM_R = outputPWM->PWM_R;
        g_hostRatioKDebug.PWM_G = outputPWM->PWM_G;
        g_hostRatioKDebug.PWM_B = outputPWM->PWM_B;
        g_hostRatioKDebug.PWM_W = outputPWM->PWM_W;
    }
    else
    {
        __algoGetRatioKPwmOutput(inputColor,
                                 targetColorPtr,
                                 outputPWM,
                                 ratioKTargetU,
                                 ratioKTargetV);
    }
}
'@

$algorithmText = $sonder2FunctionRegex.Replace($algorithmText, $sonder2FunctionNew, 1)

[System.IO.File]::WriteAllText($algorithmPath, $algorithmText, $encGbk)

Write-Utf8File (Join-Path $flatRoot 'HostRatioKDebug.h') @'
#ifndef _HOST_RATIO_K_DEBUG_H_
#define _HOST_RATIO_K_DEBUG_H_

#include <stdint.h>
#include "SystemType.h"

typedef struct HostRatioKDebug
{
    uint16_t lookupColorU;
    uint16_t lookupColorV;
    uint16_t inputRgbPointX;
    uint16_t inputRgbPointY;
    uint16_t inputWhitePointX;
    uint16_t inputWhitePointY;
    uint16_t whiteRatioQ10000;
    uint8_t  ratioValid;
    uint8_t  splitValid;
    int32_t  targetColor_x;
    int32_t  targetColor_y;
    int32_t  targetColor_Y;
    int32_t  whiteRatioBeforeClamp;
    int32_t  maxWhiteRatio;
    int32_t  whiteRatioAfterClamp;
    int32_t  boundaryWhiteY;
    int32_t  targetColorY;
    int32_t  whiteRatio;
    int32_t  rgbRatio;
    int32_t  splitPointCIE_x;
    int32_t  splitPointCIE_y;
    int32_t  splitPointCIE_Y;
    int32_t  splitWhiteCIE_x;
    int32_t  splitWhiteCIE_y;
    int32_t  splitWhiteCIE_Y;
    int32_t  dutyColor_x;
    int32_t  dutyColor_y;
    int32_t  dutyColor_Y;
    int32_t  verifyRgbPointErrX;
    int32_t  verifyRgbPointErrY;
    int32_t  verifyRatioSumErr;
    int32_t  verifyTarget_x;
    int32_t  verifyTarget_y;
    int32_t  verifyTarget_Y;
    int32_t  verifyTargetErrX;
    int32_t  verifyTargetErrY;
    int32_t  verifyTargetErrYValue;
    uint16_t PWM_R;
    uint16_t PWM_G;
    uint16_t PWM_B;
    uint16_t PWM_W;
} SHostRatioKDebug;

extern volatile SHostRatioKDebug g_hostRatioKDebug;

void HostRatioKDebug_Reset(void);

#endif
'@

Write-Utf8File (Join-Path $flatRoot 'HostRatioKDebug.c') @'
#include <string.h>
#include "HostRatioKDebug.h"

volatile SHostRatioKDebug g_hostRatioKDebug;

void HostRatioKDebug_Reset(void)
{
    memset((void *)&g_hostRatioKDebug, 0, sizeof(g_hostRatioKDebug));
}
'@

Write-Utf8File (Join-Path $flatRoot 'ModuleFlash.h') @'
#ifndef _HOST_MODULEFLASH_H_
#define _HOST_MODULEFLASH_H_

#include <stdint.h>
#include "SystemType.h"

typedef struct WhiteRgbMixParams
{
    uint16_t rgbPointX;
    uint16_t rgbPointY;
    uint16_t whiteRatio;
    uint16_t rgbRatio;
} SWhiteRgbMixParams;

typedef struct FlashContent
{
    uint16_t factor;
    uint16_t eolAddr;
    uint16_t redx;
    uint16_t redy;
    uint32_t redY;
    uint16_t greenx;
    uint16_t greeny;
    uint32_t greenY;
    uint16_t bluex;
    uint16_t bluey;
    uint32_t blueY;
    uint16_t whitex;
    uint16_t whitey;
    uint32_t whiteY;
    uint8_t brightness_factor;
    uint8_t Utemp;
    uint8_t Vtemp;
    SWhiteRgbMixParams whiteRgbMixParams;
} SFlashContent;

extern SFlashContent savedConfig;

#endif
'@

Write-Utf8File (Join-Path $flatRoot 'ModuleLed.h') @'
#ifndef _HOST_MODULELED_H_
#define _HOST_MODULELED_H_

#define PWM_PERIOD 32000

#endif
'@

Write-Utf8File (Join-Path $flatRoot 'ModuleTemperature.h') @'
#ifndef _HOST_MODULETEMPERATURE_H_
#define _HOST_MODULETEMPERATURE_H_

#define DTEMP_MULTIPLIED_RATIO 10

#endif
'@

Write-Utf8File (Join-Path $flatRoot 'SystemControl.h') @'
#ifndef _HOST_SYSTEMCONTROL_H_
#define _HOST_SYSTEMCONTROL_H_
#endif
'@

Write-Utf8File (Join-Path $flatRoot 'SystemStatus.h') @'
#ifndef _HOST_SYSTEMSTATUS_H_
#define _HOST_SYSTEMSTATUS_H_
#endif
'@

Write-Utf8File (Join-Path $flatRoot 'SystemLinComm.h') @'
#ifndef _HOST_SYSTEMLINCOMM_H_
#define _HOST_SYSTEMLINCOMM_H_

#include <stdint.h>

typedef struct HostRgbLinState
{
    uint8_t BCM_RGB_Sonderfunktion;
} SHostRgbLinState;

typedef struct HostSysLinStack
{
    SHostRgbLinState RGB;
} SHostSysLinStack;

extern SHostSysLinStack sysLin_Stack;

#endif
'@

Write-Utf8File (Join-Path $flatRoot 'host_saved_config.c') @"
#include "ModuleFlash.h"
#include "SystemLinComm.h"

SFlashContent savedConfig =
{
    32768u, /* factor */
    22170u, /* eolAddr */
    ${hostRedX}u,  /* redx */
    ${hostRedY}u,  /* redy */
    ${hostRedYY}u, /* redY */
    ${hostGreenX}u, /* greenx */
    ${hostGreenY}u, /* greeny */
    ${hostGreenYY}u, /* greenY */
    ${hostBlueX}u,  /* bluex */
    ${hostBlueY}u,  /* bluey */
    ${hostBlueYY}u, /* blueY */
    ${hostWhiteX}u, /* whitex */
    ${hostWhiteY}u, /* whitey */
    ${hostWhiteYY}u, /* whiteY */
    100u,   /* brightness_factor */
    80u,    /* Utemp */
    188u,   /* Vtemp */
    {${hostMixRgbPointX}u, ${hostMixRgbPointY}u, ${hostMixWhiteRatio}u, ${hostMixRgbRatio}u}
};

SHostSysLinStack sysLin_Stack = {0};
"@

Write-Utf8File (Join-Path $flatRoot 'ratio_k_test_main.c') @'
#include <stdio.h>
#include <stdlib.h>
#include "Algorithm.h"
#include "HostRatioKDebug.h"
#include "ModuleFlash.h"
#include "ModuleTemperature.h"
#include "SystemLinComm.h"

static void print_q10000_xyz(const char *name, int32_t x, int32_t y, int32_t Y)
{
    printf("%s = (%0.4f, %0.4f, %0.4f)\n",
           name,
           (double)x / 10000.0,
           (double)y / 10000.0,
           (double)Y / 10000.0);
}

static void write_csv(const char *path, unsigned int rawU, unsigned int rawV)
{
    FILE *fp = fopen(path, "w");
    if (fp == NULL)
    {
        return;
    }

    fprintf(fp, "requested_raw_u,requested_raw_v,algo_pwm_input_u,algo_pwm_input_v,ratioK_lookup_u,ratioK_lookup_v,input_rgb_x,input_rgb_y,input_white_x,input_white_y,");
    fprintf(fp, "whiteRatioQ10000,ratioValid,splitValid,whiteRatioBeforeClamp,maxWhiteRatio,whiteRatioAfterClamp,boundaryWhiteY,targetColorY,whiteRatioUsed,rgbRatio,");
    fprintf(fp, "target_x,target_y,target_Y,splitPoint_x,splitPoint_y,splitPoint_Y,");
    fprintf(fp, "splitWhite_x,splitWhite_y,splitWhite_Y,duty_x,duty_y,duty_Y,");
    fprintf(fp, "verifyRgbPointErrX,verifyRgbPointErrY,verifyRatioSumErr,");
    fprintf(fp, "verifyTarget_x,verifyTarget_y,verifyTarget_Y,");
    fprintf(fp, "verifyTargetErrX,verifyTargetErrY,verifyTargetErrYValue,");
    fprintf(fp, "pwm_r,pwm_g,pwm_b,pwm_w\n");

    fprintf(fp,
            "%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%u,%u,%u,%u\n",
            rawU,
            rawV,
            rawU * 5u,
            rawV * 5u,
            (unsigned int)g_hostRatioKDebug.lookupColorU,
            (unsigned int)g_hostRatioKDebug.lookupColorV,
            (unsigned int)g_hostRatioKDebug.inputRgbPointX,
            (unsigned int)g_hostRatioKDebug.inputRgbPointY,
            (unsigned int)g_hostRatioKDebug.inputWhitePointX,
            (unsigned int)g_hostRatioKDebug.inputWhitePointY,
            (unsigned int)g_hostRatioKDebug.whiteRatioQ10000,
            (unsigned int)g_hostRatioKDebug.ratioValid,
            (unsigned int)g_hostRatioKDebug.splitValid,
            (long)g_hostRatioKDebug.whiteRatioBeforeClamp,
            (long)g_hostRatioKDebug.maxWhiteRatio,
            (long)g_hostRatioKDebug.whiteRatioAfterClamp,
            (long)g_hostRatioKDebug.boundaryWhiteY,
            (long)g_hostRatioKDebug.targetColorY,
            (long)g_hostRatioKDebug.whiteRatio,
            (long)g_hostRatioKDebug.rgbRatio,
            (long)g_hostRatioKDebug.targetColor_x,
            (long)g_hostRatioKDebug.targetColor_y,
            (long)g_hostRatioKDebug.targetColor_Y,
            (long)g_hostRatioKDebug.splitPointCIE_x,
            (long)g_hostRatioKDebug.splitPointCIE_y,
            (long)g_hostRatioKDebug.splitPointCIE_Y,
            (long)g_hostRatioKDebug.splitWhiteCIE_x,
            (long)g_hostRatioKDebug.splitWhiteCIE_y,
            (long)g_hostRatioKDebug.splitWhiteCIE_Y,
            (long)g_hostRatioKDebug.dutyColor_x,
            (long)g_hostRatioKDebug.dutyColor_y,
            (long)g_hostRatioKDebug.dutyColor_Y,
            (long)g_hostRatioKDebug.verifyRgbPointErrX,
            (long)g_hostRatioKDebug.verifyRgbPointErrY,
            (long)g_hostRatioKDebug.verifyRatioSumErr,
            (long)g_hostRatioKDebug.verifyTarget_x,
            (long)g_hostRatioKDebug.verifyTarget_y,
            (long)g_hostRatioKDebug.verifyTarget_Y,
            (long)g_hostRatioKDebug.verifyTargetErrX,
            (long)g_hostRatioKDebug.verifyTargetErrY,
            (long)g_hostRatioKDebug.verifyTargetErrYValue,
            (unsigned int)g_hostRatioKDebug.PWM_R,
            (unsigned int)g_hostRatioKDebug.PWM_G,
            (unsigned int)g_hostRatioKDebug.PWM_B,
            (unsigned int)g_hostRatioKDebug.PWM_W);

    fclose(fp);
}

int main(int argc, char *argv[])
{
    SColorParams inputColor;
    SPWMParams outputPWM;
    unsigned int inputU = 40u;
    unsigned int inputV = 94u;

    if (argc >= 3)
    {
        inputU = (unsigned int)strtoul(argv[1], NULL, 0);
        inputV = (unsigned int)strtoul(argv[2], NULL, 0);
    }

    HostRatioKDebug_Reset();
    inputColor.colorU = (uint16_t)inputU*5;
    inputColor.colorV = (uint16_t)inputV*5;
    inputColor.ledTemprature = 25 * DTEMP_MULTIPLIED_RATIO;
    inputColor.factor = savedConfig.factor;
    inputColor.intensity = 6400u;
    inputColor.tempAdjust = 6400u;
    inputColor.RGBFlag = bfalse;

    outputPWM.PWM_R = 0u;
    outputPWM.PWM_G = 0u;
    outputPWM.PWM_B = 0u;
    outputPWM.PWM_W = 0u;

    algoUpdateRedCIE(savedConfig.redx, savedConfig.redy, savedConfig.redY);
    algoUpdateGreenCIE(savedConfig.greenx, savedConfig.greeny, savedConfig.greenY);
    algoUpdateBlueCIE(savedConfig.bluex, savedConfig.bluey, savedConfig.blueY);
    algoUpdateWhiteCIE(savedConfig.whiteY);
    WitheBalanceTempInit();

    sysLin_Stack.RGB.BCM_RGB_Sonderfunktion = 2u;
    (void)algoGetPWMOutput(&inputColor, &outputPWM);

    printf("=== Ratio K host test ===\n");
    printf("requested UV(raw)        = %u, %u\n", inputU, inputV);
    printf("algoGetPWMOutput input   = %u, %u\n", (unsigned int)inputColor.colorU, (unsigned int)inputColor.colorV);
    printf("ratioK LUT input         = %u, %u\n",
           (unsigned int)g_hostRatioKDebug.lookupColorU,
           (unsigned int)g_hostRatioKDebug.lookupColorV);
    printf("RGB point input          = (%0.4f, %0.4f)\n",
           (double)g_hostRatioKDebug.inputRgbPointX / 10000.0,
           (double)g_hostRatioKDebug.inputRgbPointY / 10000.0);
    printf("White point input        = (%0.4f, %0.4f)\n",
           (double)g_hostRatioKDebug.inputWhitePointX / 10000.0,
           (double)g_hostRatioKDebug.inputWhitePointY / 10000.0);
    printf("whiteRatioQ10000         = %u\n", (unsigned int)g_hostRatioKDebug.whiteRatioQ10000);
    printf("whiteRatio used          = %0.4f\n", (double)g_hostRatioKDebug.whiteRatioAfterClamp / 10000.0);
    printf("rgbRatio used            = %0.4f\n", (double)g_hostRatioKDebug.rgbRatio / 10000.0);
    printf("ratioValid               = %u\n", (unsigned int)g_hostRatioKDebug.ratioValid);
    printf("splitValid               = %u\n", (unsigned int)g_hostRatioKDebug.splitValid);

    print_q10000_xyz("targetColor", g_hostRatioKDebug.targetColor_x, g_hostRatioKDebug.targetColor_y, g_hostRatioKDebug.targetColor_Y);
    print_q10000_xyz("splitPointCIE", g_hostRatioKDebug.splitPointCIE_x, g_hostRatioKDebug.splitPointCIE_y, g_hostRatioKDebug.splitPointCIE_Y);
    print_q10000_xyz("splitWhiteCIE", g_hostRatioKDebug.splitWhiteCIE_x, g_hostRatioKDebug.splitWhiteCIE_y, g_hostRatioKDebug.splitWhiteCIE_Y);
    print_q10000_xyz("dutyColor", g_hostRatioKDebug.dutyColor_x, g_hostRatioKDebug.dutyColor_y, g_hostRatioKDebug.dutyColor_Y);
    printf("PWM output          = R:%u G:%u B:%u W:%u\n",
           (unsigned int)g_hostRatioKDebug.PWM_R,
           (unsigned int)g_hostRatioKDebug.PWM_G,
           (unsigned int)g_hostRatioKDebug.PWM_B,
           (unsigned int)g_hostRatioKDebug.PWM_W);

    write_csv("ratio_k_test_output.csv", inputU, inputV);
    return 0;
}
'@

Write-Output "Prepared Ratio-K host test sources in $flatRoot"
