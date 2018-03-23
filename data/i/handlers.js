function prepareAnswer(el, ms) {
    $(el).prop('disabled', true);
    setTimeout(function() {
        $(el).prop('disabled', false);
    }, ms);
}

// drop-down menu
// $.fx.off = true;
$('#btn-menu').on('click', function() {
    if(!$('#menu').hasClass('active')) {
        $('#menu').addClass('active');
        // $('#menu').animate('activate-menu');
    } else {
        $('#menu').removeClass('active');
        // $('#menu').animate('deactivate-menu');
    }
})

// handler menu links
$('#menu a').on('click', function(e) {
    e.preventDefault();
    $('#menu').removeClass('active');
    $.get($(this).attr('href'), function(data) {
        $('main').html(data);
    })
})

// handler all buttons (who have cmd attr)
$('main').on('click', 'button[cmd]', function(){
    prepareAnswer($('input[off], button'), 1000);
    sock.send($(this).attr('cmd'));
    console.log($(this).attr('cmd'));
})

// handler all switches
$('main').on('click', 'input[off]', function(e) {
    prepareAnswer($('input[off], button'), 1000);
    if($(this).prop('checked')) {
        console.log($(this).attr('off'));
        sock.send($(this).attr('off'));
    } else {
        console.log($(this).attr('on'));
        sock.send($(this).attr('on'));
    }
})