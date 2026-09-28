# GXDE 热区

GXDE热区是对Deepin 15时代热区的复刻，它利用了屏幕的四个角落，鼠标移动到屏幕角落便可触发用户自行设置的对应功能。

<br />

现在GXDE热区同时支持X11和Wayland两种协议。

![输入图片说明](doc/imgs/screenshot1.png "输入图片说明")![输入图片说明](doc/imgs/screenshot2.png "输入图片说明")

<br />

## 构建并运行

### 使用CMake构建

您需要确保您的系统上安装了Qt6与LayerShellQt的开发包，然后可以执行：

```shell
$ mkdir build && cd build
$ cmake ..
$ make
```

<br />

### Debian打包

可以使用`build-deb`在本地构建Debian包，使用方式很简单：

```Shell
$ chmod a+x ./build-deb
$ ./build-deb -d  # 会自动下载安装依赖，下一次就可以不带这个-d参数运行了
$ ./build-deb -c  # 清理仓库用的，注意会同时删除生成的deb包
```

<br />

## 配置

### 配置文件

配置文件可以在 `$HOME/.config/GXDE/gxde-requ/gxde-requ-setting.qaq`找到，它的格式为：

```ini
TopLeftShell=
TopRightShell=
LowerLeftShell=
LowerRightShell=
```

<br />

这里有一个例子：

```ini
TopLeftShell=dbus-send --print-reply --dest=com.deepin.dde.Launcher /com/deepin/dde/Launcher com.deepin.dde.Launcher.Toggle
TopRightShell=/usr/lib/deepin-daemon/desktop-toggle
```

<br />

### 使用命令行工具配置

> 在其他桌面，可通过执行`gxde-requ-setter`以配置热区

<br />

`gxde-requ-setter`的使用方法如下：

```shell
$ /usr/bin/gxde-requ-setter --set <热角> <命令>  # 修改指定热角的命令
$ /usr/bin/gxde-requ-setter --clear <热角>       # 清除指定热角的命令
$ /usr/bin/gxde-requ-setter --gui                # 调出图形化配置器
```

<br />

## 许可证

GXDE热区以GNU GENERAL PUBLIC LICENSE Version 3许可发行，您可以在[这里](./LICENSE)找到一份许可证的副本。
