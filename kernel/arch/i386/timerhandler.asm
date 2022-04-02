global timerHandler
extern increment1ms

timerHandler:
    pushad
    call increment1ms
    popad
    iret