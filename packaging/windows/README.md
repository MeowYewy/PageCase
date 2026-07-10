# Windows 发布打包指南

本目录包含 PDF Studio（`desktop-qt`）的 Windows 发布资源。

## 目录结构

```
desktop-qt/
├── scripts/
│   ├── env.bat                 # 检测 Qt / MinGW / CMake 路径
│   ├── build-release.bat       # Release 编译
│   ├── deploy.bat              # windeployqt + 复制 tools
│   ├── package-portable.bat    # 生成便携 ZIP
│   ├── package-installer.bat   # Inno Setup 安装包
│   └── release.bat             # 一键全流程
├── packaging/windows/
│   ├── PDFStudio.iss           # Inno Setup 脚本
│   ├── PORTABLE-README.txt     # 便携版说明（打入 dist）
│   └── third-party-LICENSES.txt
├── dist/
│   ├── PDFStudio-0.1.0-win64/  # 可运行目录（deploy 输出）
│   └── artifacts/              # ZIP + Setup.exe
└── APP_VERSION.txt             # 版本号（脚本读取；勿命名为 VERSION）
```

## 一键发布（本地）

```bat
cd /d D:\path\to\PDF_Studio
scripts\release.bat
```

或分步执行：

```bat
scripts\build-release.bat
scripts\deploy.bat
scripts\package-portable.bat
scripts\package-installer.bat   REM 需安装 Inno Setup 6
```

## 环境要求

| 组件 | 说明 |
|------|------|
| Qt 6.11.1 MinGW 64-bit | 含 windeployqt |
| MinGW 13.1 | 通常随 Qt 安装 |
| CMake 3.21+ | Qt Tools 自带 |
| Inno Setup 6 | 可选，用于生成 Setup.exe |
| PowerShell | 用于 Compress-Archive 打 ZIP |

可通过环境变量覆盖默认路径：

```bat
set QT_DIR=D:\Qt\6.11.1\mingw_64
set MINGW_DIR=D:\Qt\Tools\mingw1310_64
set CMAKE_DIR=D:\Qt\Tools\CMake_64\bin
```

## 产物

| 文件 | 用途 |
|------|------|
| `dist/PDFStudio-{version}-win64/` | 便携版目录，可直接运行 |
| `dist/artifacts/PDFStudio-{version}-win64-portable.zip` | GitHub 便携下载 |
| `dist/artifacts/PDFStudio-{version}-win64-Setup.exe` | 安装程序 |

## 发布到 GitHub Releases

1. 更新 `APP_VERSION.txt`、`resources/update.json`、`resources/changelog.json`
2. 运行 `scripts\release.bat`
3. 提交并打 tag：

```bat
git tag v0.1.0
git push origin v0.1.0
```

4. 上传产物（GitHub CLI 示例）：

```bat
gh release create v0.1.0 ^
  dist\artifacts\PDFStudio-0.1.0-win64-portable.zip ^
  dist\artifacts\PDFStudio-0.1.0-win64-Setup.exe ^
  --title "PDF Studio v0.1.0" ^
  --notes "首次 Windows 正式版发布。"
```

`resources/update.json` 中的 `downloadUrl` 应指向 Setup.exe 的 Release 地址。

## SmartScreen / 杀毒软件

未签名的 exe 可能触发 Windows SmartScreen。正式发布建议：

- 使用 Authenticode 代码签名证书
- 或先发布 ZIP 便携版，并在 README 中说明 SmartScreen 处理方式

## 迁移到其他电脑

复制整个 `desktop-qt` 源码目录后：

1. 安装相同版本 Qt 6.11.1 MinGW
2. 设置 `QT_DIR`（或在 `scripts\env.bat` 默认路径中修改）
3. 删除 `build/`、`dist/` 后重新运行 `scripts\release.bat`

`tools/qpdf` 与 `tools/poppler` 随仓库分发，无需重新下载（除非缺失）。
