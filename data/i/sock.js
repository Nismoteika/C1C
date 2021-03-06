var sock = new WebSocket("ws://192.168.4.1/ws");

sock.onopen = function() {
    $('#status span').text("Connected").css("color", "#409F2C");
}

sock.onclose = function() {
    $('#status span').text("Disconnected").css("color", "#B83340");
}

sock.onerror = function(err) {
    console.log(err.message);
}

sock.onmessage = function(event) {
    prepareData(event.data);
    $('#log').prepend(`<span>${event.data}</span>`);
}

$.get('main.htm', function(data) {
    $('main').html(data);
})

function prepareData(data) {
    var partData = data.split(" ");
    if(partData[1] == "00") { //status
        $('#powerBtn').css("color", "#409F2C");
    } else if (partData[1] == "20") {
        $('#powerBtn').css("color", "#BEBE35");
    } else if (partData[1] == "80") {
        $('#powerBtn').css("color", "#B83340");
    }
    if(partData[2] == "close") { //shutter
        $('#shutter').prop("checked", true);
    } else if(partData[2] == "open") {
        $('#shutter').prop("checked", false);
    }
    if(partData[3] == "freeze") { //freeze
        $('#freeze').prop("checked", true);
    } else if(partData[3] == "active") {
        $('#freeze').prop("checked", false);
    }
    //return partData.join(" ");
}