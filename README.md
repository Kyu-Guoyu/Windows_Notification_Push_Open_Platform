Copyright (C) 2016-2023 WinToast v1.3.0 - Mohammed Boujemaoui <mohabouje@gmail.com>  
Windows 操作中心通知开放平台  
  
用法:  
        Windows_Notification_Push_Open_Platform.exe 【选项】  
  
选项:  
        --aumi ：设置应用用户模型 ID*  
        --name ：设置显示应用名称*  
        --id ：设置应用ID*  
        --time ：设置显示时间（仅在0≤时间≤7000时有效）  
        --text01 ：设置通知的第一行文本  
        --text02 ：设置通知的第二行文本  
        --button ：设置通知中的按钮  
        --image-path ：设置图像路径  
        --audio-state ：设置通知音频的播放模式：单次 = 0，无 = 1，多次循环 = 2  
        --attribute ：设置通知的注释（也可当作第三行使用[Doge]）  
        --ocs ：仅创建应用的快捷方式  
        --help ：打印帮助说明  
  
注意事项及提示：  
        被“*”标记的选项为必要选项，如果缺失，程序可能无法正确运行导致通知启推异常。  
        当 --help 选项与其他选项并存时，程序会忽略其他选项，仅接受 --help 选项。
