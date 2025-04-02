# Dmenu Polkit Agent

Polkit agent that uses dmenu as a prompt.

# Usage

Add this command to your compositor startup script or `.xinitrc`:
`dmenu-polkit`

```
    git clone --depth=1 https://github.com/gadefox/dmenu-polkit.git
    cd dmenu-polkit
    meson setup build --buildtype=release
    cd build
    meson install
```

## Special thanks to [holopk](https://github.com/czadowanie/holopk)
