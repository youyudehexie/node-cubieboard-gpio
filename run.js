var GPIO = require('./build/Release/gpio');

var LED = GPIO.PD0;
var status = 0;
GPIO.init();

GPIO.setcfg(LED, GPIO.OUT);

// 让led 一闪一闪
var blink = function(){
    if(status){
	GPIO.output(LED, GPIO.LOW);
	status = 0;
    } else {
	GPIO.output(LED, GPIO.HIGH);
	status = 1;
    }
}

setInterval(blink, 1000);





