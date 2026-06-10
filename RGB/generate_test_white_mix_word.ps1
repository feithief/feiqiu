﻿$ErrorActionPreference = 'Stop'

function Add-Title {
    param(
        $Document,
        [string]$Text
    )

    $paragraph = $Document.Paragraphs.Add()
    $paragraph.Range.Text = $Text
    $paragraph.Range.Font.NameFarEast = 'Microsoft YaHei'
    $paragraph.Range.Font.Name = 'Times New Roman'
    $paragraph.Range.Font.Size = 18
    $paragraph.Range.Font.Bold = 1
    $paragraph.Range.ParagraphFormat.Alignment = 1
    $paragraph.Range.InsertParagraphAfter() | Out-Null
}

function Add-Heading {
    param(
        $Document,
        [string]$Text,
        [int]$Level = 1
    )

    $paragraph = $Document.Paragraphs.Add()
    $paragraph.Range.Text = $Text
    $paragraph.Range.Style = "Heading $Level"
    $paragraph.Range.Font.NameFarEast = 'Microsoft YaHei'
    $paragraph.Range.Font.Name = 'Times New Roman'
    $paragraph.Range.InsertParagraphAfter() | Out-Null
}

function Add-Body {
    param(
        $Document,
        [string]$Text
    )

    $paragraph = $Document.Paragraphs.Add()
    $paragraph.Range.Text = $Text
    $paragraph.Range.Font.NameFarEast = 'Microsoft YaHei'
    $paragraph.Range.Font.Name = 'Times New Roman'
    $paragraph.Range.Font.Size = 11
    $paragraph.Range.ParagraphFormat.Alignment = 0
    $paragraph.Range.InsertParagraphAfter() | Out-Null
}

function Add-Equation {
    param(
        $Document,
        [string]$LinearText
    )

    $paragraph = $Document.Paragraphs.Add()
    $paragraph.Range.Text = $LinearText
    $paragraph.Range.Font.Name = 'Cambria Math'
    $paragraph.Range.Font.Size = 14
    $paragraph.Range.ParagraphFormat.Alignment = 1
    [void]$Document.OMaths.Add($paragraph.Range)
    $paragraph.Range.InsertParagraphAfter() | Out-Null
}

$outputPath = 'E:\AUDI_COLOR\RGB\test_white_mix_数学推导.docx'

$word = New-Object -ComObject Word.Application
$word.Visible = $false
$word.DisplayAlerts = 0
$document = $word.Documents.Add()

try {
    Add-Title $document 'test_white_mix.m 数学推导'

    Add-Body $document '本文对应当前 RGB 文件夹下的 test_white_mix.m、xyY_PWM.m、RGB_to_xyY_32000.m 的实际实现流程。'
    Add-Body $document '文中所有公式均使用 Word 专业公式对象生成，打开后应为标准数学排版格式。'

    Add-Heading $document '1. 已知量' 1
    Add-Equation $document 'R=(x_r,y_r,Y_r)=(0.69,0.31,1.40)'
    Add-Equation $document 'G=(x_g,y_g,Y_g)=(0.17,0.72,2.20)'
    Add-Equation $document 'B=(x_b,y_b,Y_b)=(0.15,0.03,0.50)'
    Add-Equation $document 'W=(x_W,y_W)=(0.3090,0.3176)'
    Add-Equation $document 'MaxPWM=32000'
    Add-Equation $document 'T=(x_T,y_T,Y_T)'
    Add-Equation $document 'T_(xy)=(x_T,y_T)'

    Add-Heading $document '2. 白点射线与色域交点' 1
    Add-Body $document '从白点 W 出发，经过目标点 T_(xy) 作射线，并与色域三角形边界求交。'
    Add-Equation $document 'L(t)=W+t(T_(xy)-W), t≥1'
    Add-Equation $document 'E_i(s)=A_i+s(B_i-A_i), 0≤s≤1'
    Add-Equation $document 'W+t(T_(xy)-W)=A_i+s(B_i-A_i)'
    Add-Equation $document 'P=(x_P,y_P)'
    Add-Body $document '选取满足 t≥1 且 0≤s≤1 的最小 t，对应的交点即为有效交点 P。'

    Add-Heading $document '3. 共线参数' 1
    Add-Equation $document 'k_1=|P-T_(xy)|'
    Add-Equation $document 'k_2=|T_(xy)-W|'
    Add-Equation $document 'k_3=|P-W|'
    Add-Equation $document 'λ=(k_2)/(k_3)'
    Add-Equation $document '1-λ=(k_1)/(k_3)'
    Add-Equation $document 'T_(xy)=(1-λ)W+λP'
    Add-Equation $document 'x_T=(1-λ)x_W+λx_P'
    Add-Equation $document 'y_T=(1-λ)y_W+λy_P'

    Add-Heading $document '4. 目标亮度分解' 1
    Add-Body $document '根据当前实现中的亮度推导，将目标亮度 Y_T 分解为白点分量亮度与交点分量亮度。'
    Add-Equation $document 'Y_W=Y_T ((1-λ)y_W)/(y_T)'
    Add-Equation $document 'Y_P=Y_T (λy_P)/(y_T)'
    Add-Equation $document 'Y_W+Y_P=Y_T'
    Add-Equation $document 'W''=(x_W,y_W,Y_W)'
    Add-Equation $document 'P''=(x_P,y_P,Y_P)'

    Add-Heading $document '5. xyY_PWM.m 的第一步：求三基色系数 D' 1
    Add-Equation $document 'z_r=1-x_r-y_r'
    Add-Equation $document 'z_g=1-x_g-y_g'
    Add-Equation $document 'z_b=1-x_b-y_b'
    Add-Equation $document 'z_C=1-x_C-y_C'
    Add-Equation $document 'm_(11)=((Y_r)/(y_r))x_r'
    Add-Equation $document 'm_(12)=((Y_g)/(y_g))x_g'
    Add-Equation $document 'm_(13)=((Y_b)/(y_b))x_b'
    Add-Equation $document 'm_(21)=((Y_r)/(y_r))y_r=Y_r'
    Add-Equation $document 'm_(22)=((Y_g)/(y_g))y_g=Y_g'
    Add-Equation $document 'm_(23)=((Y_b)/(y_b))y_b=Y_b'
    Add-Equation $document 'm_(31)=((Y_r)/(y_r))z_r'
    Add-Equation $document 'm_(32)=((Y_g)/(y_g))z_g'
    Add-Equation $document 'm_(33)=((Y_b)/(y_b))z_b'
    Add-Equation $document 'M=[m_(11),m_(12),m_(13);m_(21),m_(22),m_(23);m_(31),m_(32),m_(33)]'
    Add-Equation $document 'D=(D_r,D_g,D_b)^T'
    Add-Equation $document 'D=M^(-1)(x_C,y_C,z_C)^T'
    Add-Body $document '在代码中，C 可以取白点分量 W''，也可以取交点分量 P''。'

    Add-Heading $document '6. xyY_PWM.m 的第二步：由 D 求最终 PWM' 1
    Add-Equation $document 'L_C=Y_rD_r+Y_gD_g+Y_bD_b'
    Add-Equation $document 'R_C=(Y_C)/(L_C) MaxPWM D_r'
    Add-Equation $document 'G_C=(Y_C)/(L_C) MaxPWM D_g'
    Add-Equation $document 'B_C=(Y_C)/(L_C) MaxPWM D_b'
    Add-Equation $document 'PWM_W=(R_W,G_W,B_W)^T'
    Add-Equation $document 'PWM_P=(R_P,G_P,B_P)^T'

    Add-Heading $document '7. 两束光的 PWM 合成' 1
    Add-Equation $document 'PWM_(mix)=PWM_W+PWM_P'
    Add-Equation $document 'R_(mix)=R_W+R_P'
    Add-Equation $document 'G_(mix)=G_W+G_P'
    Add-Equation $document 'B_(mix)=B_W+B_P'

    Add-Heading $document '8. 直接送入 RGB_to_xyY_32000.m 的回算输入' 1
    Add-Body $document '当前版本不再把 PWM 换算成等效 0–255 RGB，而是直接使用 0–32000 标尺的 PWM 作为回算输入。'
    Add-Equation $document 'BackInput=(R_(mix),G_(mix),B_(mix))^T'

    Add-Heading $document '9. RGB_to_xyY_32000.m 的回算验证' 1
    Add-Body $document '记直接调用 RGB_to_xyY_32000.m 后得到的验证点为 V=(x_V,y_V,Y_V)。'
    Add-Equation $document 'S=((Y_r)/(y_r))R_(mix)+((Y_g)/(y_g))G_(mix)+((Y_b)/(y_b))B_(mix)'
    Add-Equation $document 'x_V=((((Y_r)/(y_r))x_rR_(mix))+(((Y_g)/(y_g))x_gG_(mix))+(((Y_b)/(y_b))x_bB_(mix)))/(S)'
    Add-Equation $document 'y_V=((Y_rR_(mix))+(Y_gG_(mix))+(Y_bB_(mix)))/(S)'
    Add-Equation $document 'Y_V=((Y_rR_(mix))+(Y_gG_(mix))+(Y_bB_(mix)))/(32000)'

    Add-Heading $document '10. 误差验证' 1
    Add-Body $document '以下的 u 与 v 表示 CIE 1976 u''v'' 坐标。'
    Add-Equation $document 'u_T=(4x_T)/(-2x_T+12y_T+3)'
    Add-Equation $document 'v_T=(9y_T)/(-2x_T+12y_T+3)'
    Add-Equation $document 'u_V=(4x_V)/(-2x_V+12y_V+3)'
    Add-Equation $document 'v_V=(9y_V)/(-2x_V+12y_V+3)'
    Add-Equation $document 'Δuv=((u_V-u_T)^2+(v_V-v_T)^2)^((1)/(2))'
    Add-Equation $document 'ΔY=|Y_V-Y_T|'
    Add-Equation $document 'δ_Y=(ΔY)/(Y_T)'

    Add-Heading $document '11. 过程总式' 1
    Add-Body $document '把 test_white_mix.m 的整个计算过程压缩成一条链，可写为：'
    Add-Equation $document 'T→(W,P,λ)→(Y_W,Y_P)→(W'',P'')→(PWM_W,PWM_P)→PWM_(mix)→(x_V,y_V,Y_V)'

    $document.OMaths.BuildUp()
    $document.SaveAs2($outputPath, 16)
}
finally {
    if ($document -ne $null) {
        $document.Close()
        [System.Runtime.Interopservices.Marshal]::ReleaseComObject($document) | Out-Null
    }
    if ($word -ne $null) {
        $word.Quit()
        [System.Runtime.Interopservices.Marshal]::ReleaseComObject($word) | Out-Null
    }
    [gc]::Collect()
    [gc]::WaitForPendingFinalizers()
}

Write-Output "Saved: $outputPath"
