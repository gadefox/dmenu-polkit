# Dmenu Polkit Agent

Polkit agent that uses dmenu as a prompt.

## MAY THEE BE WARNED: 
As of right now this is very much a proof of concept, I'm not sure
how secure is it to use this thing. Also it might just crash or leak memory.

# Usage

Add this command to your compositor startup script or `.xinitrc`:  
`dmenu-polkit`

```
    git clone --depth=1 https://github.com/gadefox/dmenu-polkit.git
    cd dmenu
    meson setup build --buildtype=release
    cd build
    meson install
```

## Special thanks to [holopk](https://github.com/czadowanie/holopk)
