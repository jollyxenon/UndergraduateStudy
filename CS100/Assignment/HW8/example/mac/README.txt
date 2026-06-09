I Zombie Mac 标准实现运行包（兼容修正版）

使用方法：
1. 双击 I Zombie.app 启动游戏。
2. 如果 macOS 提示来自未识别开发者：右键 I Zombie.app，选择“打开”，再确认打开。
3. 请保持 I Zombie.app 内部文件完整，不要删除 Contents/Resources/assets。

系统要求：
- Intel Mac：macOS 10.15 或更高版本。
- Apple Silicon Mac：macOS 11.0 或更高版本。

说明：
- 本包只包含 macOS 可运行版本和运行素材，不包含标准实现源码。
- 修复点：显式设置 macOS deployment target，避免在旧系统上显示“版本不能与此版本 macOS 配合使用”。
