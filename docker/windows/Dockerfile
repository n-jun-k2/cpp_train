FROM microsoft/dotnet-framework:4.7.1


WORKDIR /TMP
RUN powershell Invoke-WebRequest "https://download.visualstudio.microsoft.com/download/pr/11503713/e64d79b40219aea618ce2fe10ebd5f0d/vs_BuildTools.exe" -OutFile "vs_BuildTools.exe" \
    & vs_BuildTools.exe --quiet --wait --nocache --norestart --installPath C:\\BuildTools --add Microsoft.VisualStudio.Workload.VCTools \
    & DEL vs_buildtools.exe

WORKDIR /TEMP
RUN DEL /Q C:\TMP

# install
#RUN mkdir TEMP
#   cd TEMP
#   curl -O https://visualstudio.microsoft.com/thank-you-downloading-visual-studio/?sku=community&rel=16&utm_medium=microsoft&utm_source=docs.microsoft.com&utm_campaign=link+cta&utm_content=download+commandline+parameters+vs2019+rc
#   curl -O https://visualstudio.microsoft.com/8302aac7-ef59-4a5a-90e7-e4af54f6368d/vs_buildtools__1671944944.1602301289.exe
#   curl -O https://download.microsoft.com/download/5/f/7/5f7acaeb-8363-451f-9425-68a90f98b238/visualcppbuildtools_full.exe
#   curl -o vs_community.exe https://visualstudio.microsoft.com/e649c1f1-8898-4863-a767-c1d588851e2e/vs_community__1671944944.1602301289.exe
#   curl -O https://download.visualstudio.microsoft.com/download/pr/11503713/e64d79b40219aea618ce2fe10ebd5f0d/vs_BuildTools.exe


#C:\TEMP\Install.cmd C:\TEMP\vs_buildtools.exe --quiet --wait --norestart --nocache `
#    --installPath C:\BuildTools `
#    --channelUri C:\TEMP\VisualStudio.chman `
#    --installChannelUri C:\TEMP\VisualStudio.chman `
#    --add Microsoft.VisualStudio.Workload.AzureBuildTools `
#    --remove Microsoft.VisualStudio.Component.Windows10SDK.10240 `
#    --remove Microsoft.VisualStudio.Component.Windows10SDK.10586 `
#    --remove Microsoft.VisualStudio.Component.Windows10SDK.14393 `
#    --remove Microsoft.VisualStudio.Component.Windows81SDK

#start /wait visualcppbuildtools_full.exe --installPath "C:\VS" --passive --wait > nul
#echo %errorlevel%

#tasklist /V /fi "Imagename eq  vs_BuildTools.exe"

#tasklist /v /fi "STATUS eq running