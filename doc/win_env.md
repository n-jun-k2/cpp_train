# Windows images for Docker
- [windows container](https://docs.docker.com/docker-for-windows/#switch-between-windows-and-linux-containers)
# How to run...
- 拡張機能の有効化（Windowsコンテナを起動する場合は必須）
    ```powershell
    # コンテナの有効化
    dism.exe /online /enable-feature /featurename:Containers /all /norestart 
    dism.exe /online /enable-feature /featurename:Containers-SDN /all /norestart
    # Hyper-Vの有効化
    dism.exe /online /enable-feature /featurename:Microsoft-Hyper-V-All /all /norestart
    ```
- Dockerメニューの「Switch to Windows containers ...」で切り替えます。
- Windowsコンテナを立ち上げる
    ```powershell
    docker-compose up -d
    # powershellかcmdでログインする
    docker-compose exec dev_win <powershell or cmd>
    ```

# Windowsコンテナのバージョン指定
Windowsコンテナを使用する際はバージョン指定が必須です。
バージョンの確認は``winver``で確認します。
またイメージの対応バージョンを確認する。
- [Windows image version](https://mcrflowprodcentralus.data.mcr.microsoft.com/mcrprod/windows?P1=1602301288&P2=1&P3=1&P4=jDWZ3%2FNd6PD0nwiqmxh%2B07m2y2sQhHfsOXVCyuKyRxI%3D&se=2020-10-10T03%3A41%3A28Z&sig=6piHmxdJfpP58%2BGIWjV%2BH95lNqtvLExeVt24GW27C%2Bw%3D&sp=r&sr=b&sv=2015-02-21)
- [Windows Insider image version](https://mcrflowprodcentralus.data.mcr.microsoft.com/mcrprod/windows/insider?P1=1602300327&P2=1&P3=1&P4=yrhGQWgqOXZAlsVK%2BF%2FGTRmJ4Vr2PWOWtHqXMysKurc%3D&se=2020-10-10T03%3A25%3A27Z&sig=8d0k%2FkWgoJF1Cc1mmEFQ1np7J8EF1z3czwFzPqslDEQ%3D&sp=r&sr=b&sv=2015-02-21)

```dockerfile
# 通常のwindows版の指定
FROM mcr.microsoft.com/windows:1909-amd64

# insider版はOSのビルド番号を指定。バージョン2004(OSビルド 20201.1000)
FROM mcr.microsoft.com/windows/insider:10.0.20201.1000-amd64
```
