const web_socket_server = new WebSocket("ws://192.168.43.214:9000/");
var servo_periods = {
    "servo_0": 1500,
    "servo_1": 1500,
    "servo_2": 1500,
    "servo_3": 1500,
    "servo_4": 1500,
    "servo_5": 1500,
    "servo_6": 1500,
}

web_socket_server.onopen = (event) => {
    console.log("Stablished a connection sucessfuly");
    setInterval(() => {
        send_angles_message(servo_periods);
    }, 200);
}


console.log("Hello World")

$('#slider0').slider({
    min: 400,
    max: 3000,
    step: 1,
    value: 1500,
    slide: function(event, ui) {
        let value = ui.value;
        $('#slider0-value').text(value);
        servo_periods.servo_0 = value;
    }
});

$('#slider1').slider({
    min: 400,
    max: 3000,
    step: 1,
    value: 1500,
    slide: function(event, ui) {
        let value = ui.value;
        $('#slider1-value').text(value);
        servo_periods.servo_1 = value;
    }
});

$('#slider2').slider({
    min: 400,
    max: 3000,
    step: 1,
    value: 1500,
    slide: function(event, ui) {
        let value = ui.value;
        $('#slider2-value').text(value);
        servo_periods.servo_2 = value;
    }
});

$('#slider3').slider({
    min: 400,
    max: 3000,
    step: 1,
    value: 1500,
    slide: function(event, ui) {
        let value = ui.value;
        $('#slider3-value').text(value);
        servo_periods.servo_3 = value;
    }
});

$('#slider4').slider({
    min: 400,
    max: 3000,
    step: 1,
    value: 1500,
    slide: function(event, ui) {
        let value = ui.value;
        $('#slider4-value').text(value);
        servo_periods.servo_4 = value;
    }
});

$('#slider5').slider({
    min: 400,
    max: 3000,
    step: 1,
    value: 1500,
    slide: function(event, ui) {
        let value = ui.value;
        $('#slider5-value').text(value);
        servo_periods.servo_5 = value;
    }
});

function send_angles_message(servo_periods){
    const message_buffer = new ArrayBuffer(12);
    const message = new Uint16Array(message_buffer);
    message[0] = servo_periods.servo_0;
    message[1] = servo_periods.servo_1;
    message[2] = servo_periods.servo_2;
    message[3] = servo_periods.servo_3;
    message[4] = servo_periods.servo_4;
    message[5] = servo_periods.servo_5;

    web_socket_server.send(message);
}