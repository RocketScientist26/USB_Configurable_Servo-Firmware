Image "no USB.png" shows profiling results while PID is running but without
USB app polling status continuously. Image "with USB.png" shows same while
app is polling continuously status (PID is also running).

As you can see most of the time MCU runs functions in "App_Loop()" (like 
"LED_Update()", "USB_Det()", etc..), which can be interrupted even much more 
frequently with PID timer interrupt. So there is plenty of MCU time for 
running PID at even higher frequencies or for any other tasks, I do not
consider any higher PID frequency to be necessary, so limited it to 200hz.