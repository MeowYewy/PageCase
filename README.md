# PDF Studio for Windows

**简洁优雅的本地 PDF 工具箱** — 合并、拆分、旋转、转换、压缩、加水印，打开即用，文件不出本机。

<p align="center">
  <img src="resources/logo.svg" alt="PDF Studio" width="72" />
</p>

| | |
|---|---|
| **版本** | v0.1.0 |
| **平台** | Windows 10 / 11，64 位 |
| **语言** | 简体中文 · 繁體中文 · English · Français |
| **主题** | 浅色 / 深色 |
| **作者** | [MeowYewy](https://github.com/MeowYewy) |

---

## 为什么选择 PDF Studio

- **本地处理**：PDF 在电脑上完成，不上传云端，适合日常办公与隐私敏感文件
- **功能聚焦**：只做最常用的六件事，界面干净，没有臃肿菜单
- **实时预览**：添加文件后即可滚动预览；PDF 多页懒加载，大文件不卡死界面
- **细节到位**：拖放导入、合并拖拽排序、记住输出目录、处理进度与完成提示

---

## 功能一览

| 功能 | 说明 |
|------|------|
| **拆分** | 将多页 PDF 按页拆成多个单页文件 |
| **合并** | 将多个 PDF 合并为一个；支持拖拽调整顺序 |
| **旋转** | 将 PDF 全部页面旋转 90° / 180° / 270°，预览区实时显示效果 |
| **转换** | 图片、文本、Office 等转为 PDF；或将 PDF 导出为 PNG / JPEG |
| **压缩** | 三档压缩强度，减小 PDF 体积 |
| **水印** | 为每一页添加文字水印；支持 1–5 条对角线平铺、预览与导出一致 |

### 预览支持

- **PDF**：多页滚动预览（poppler / Qt PDF）
- **图片**：png、jpg、gif、webp、tiff 等
- **Office**：docx、xlsx、pptx、odt、rtf 等文字提取预览（非排版还原）
- **文本**：txt、md、csv、log 等

---

## 下载与安装

**GitHub Releases**（推荐）

| 文件 | 说明 |
|------|------|
| [PDFStudio-0.1.0-win64-Setup.exe](https://github.com/MeowYewy/PDF-Studio-for-Windows/releases/download/v0.1.0/PDFStudio-0.1.0-win64-Setup.exe) | 安装版（首次安装有向导界面） |
| [PDFStudio-0.1.0-win64-portable.zip](https://github.com/MeowYewy/PDF-Studio-for-Windows/releases/download/v0.1.0/PDFStudio-0.1.0-win64-portable.zip) | 便携版（解压即用） |

**系统要求**

- Windows 10 / 11，64 位
- 无需单独安装 Qt 运行时（Release 包已内置依赖）

> 首次运行若出现 SmartScreen 提示，选择「更多信息」→「仍要运行」。正式分发建议对安装包进行代码签名。

**在线更新**

- **首次安装**：从 GitHub 下载 `Setup.exe`，正常安装向导（有界面）
- **应用内更新**：设置 → 检查更新 → 点「新版本」，下载后**静默安装并自动重启**（可能弹出一次 UAC 授权）

`resources/update.json` 配置 Release 地址与静默安装参数。

---

## Windows 发布打包

维护者本地一键打包：

```bat
cd /d D:\path\to\PDF-Studio-for-Windows
scripts\release.bat
```

产物位于 `dist/artifacts/`。详细说明见 [`packaging/windows/README.md`](packaging/windows/README.md)。

---

## 从源码构建

### 环境

| 组件 | 版本 |
|------|------|
| Qt | 6.11.1 MinGW 64-bit |
| CMake | 3.21+ |
| 编译器 | MinGW 13.1.0 |

### Qt Creator（推荐）

1. 打开 `CMakeLists.txt`
2. Kit 选择 **Desktop Qt 6.11.1 MinGW 64-bit**
3. **Build** → **Run**

### 命令行

```bat
build.bat          REM 或 scripts\build-release.bat
run.bat
```

### 内置工具链

| 工具 | 用途 | 路径 |
|------|------|------|
| qpdf 12.3.2 | 合并 / 拆分 / 旋转 / 压缩 / 水印 | `tools/qpdf/` |
| poppler pdftoppm | PDF 页面预览（Qt Pdf 未安装时回退） | `tools/poppler/` |

构建时自动复制到输出目录 `tools/` 下。图片与文本转 PDF 使用 Qt 内置 `QPdfWriter`。

---

## 项目结构

```
PDF-Studio-for-Windows/
├── qml/                  # QML 界面（页面、组件、主题）
├── src/                  # C++ 引擎、预览、设置、更新检查
├── resources/            # 图标、changelog、update 清单
├── tools/                # qpdf、poppler（构建/打包时复制到输出目录）
├── scripts/              # Windows 构建与发布脚本
├── packaging/windows/    # Inno Setup、第三方许可、发布说明
├── dist/                 # 打包输出（gitignore）
├── APP_VERSION.txt       # 当前版本号（勿命名为 VERSION）
└── docs/                 # 产品介绍 / 官网文案
```

---

## 技术栈

- **UI**：Qt 6.11 + QML + Qt Quick Controls 2
- **PDF 引擎**：qpdf（CLI 封装）
- **预览**：Qt PDF（可选）/ poppler pdftoppm
- **Office 预览**：自研文本提取（docx / xlsx / pptx 等）
- **更新**：`QtNetwork` + `update.json`

---

## 路线图（节选）

- [x] Windows 安装包与 GitHub Releases 发布
- [x] 静默更新 + 自动重启（应用内更新；首次 GitHub 下载仍为安装向导）
- [ ] 处理完成后「打开输出文件夹」
- [ ] 拆分 / 旋转支持指定页码范围
- [ ] PDF 加密 / 解密

---

## 许可证

MIT

---

## 致谢

- [qpdf](https://github.com/qpdf/qpdf) — PDF 变换
- [poppler](https://poppler.freedesktop.org/) — PDF 渲染预览
- [Qt](https://www.qt.io/) — 跨平台 UI 框架

---

<p align="center"><sub>PDF Studio for Windows · MeowYewy</sub></p>
