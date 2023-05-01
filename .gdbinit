define sb
    save breakpoint .gdbbreak
end

add-symbol-file /usr/local/lib/libvulkan_lvp.so
add-symbol-file lib/libcore.so
source .gdbbreak
