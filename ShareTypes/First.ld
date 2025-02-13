SECTIONS {
    .my_global 0x1000 : {
        KEEP(*(.my_global))
    }
}
