PDF 页面预览需要以下工具之一：

## 方式 A（推荐）：安装 Poppler

在项目目录运行：

    setup-poppler.bat

成功后会生成 `pdftoppm.exe`。然后在 Qt Creator 中 **Rebuild All**，
构建脚本会自动把它复制到 exe 旁边的 `tools/poppler/`。

## 方式 B：安装 Qt Pdf 模块

1. 打开 Qt Maintenance Tool
2. 选择 Qt 6.11.1 → 勾选 **Qt Pdf**
3. 安装后 Rebuild All

若已安装 Qt Pdf，程序会优先使用内置渲染，无需 Poppler。
