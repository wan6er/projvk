define sb
    save breakpoint .gdbbreak
end

define bp
    break $arg0
    commands
        silent
        bp_cmd
        continue
    end
end

define setbp
    if $argc != 2
        return
    else
        alias bp_cmd = print $arg1
        bp $arg0 $arg1
    end
end

add-symbol-file /usr/local/lib/x86_64-linux-gnu/libvulkan_lvp.so
add-symbol-file lib/libcore.so
source .gdbbreak
