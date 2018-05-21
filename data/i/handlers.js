function prepareAnswer(el, ms) {
    $(el).prop('disabled', true);
    setTimeout(function() {
        $(el).prop('disabled', false);
    }, ms);
}

$('#btn-menu').on('click', function() {
    $('#menu').toggleClass('active');
    $('nav button').toggleClass('open');
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
    prepareAnswer($('input[off], button'), 1250);
    sock.send($(this).attr('cmd'));
})

// handler all switches
$('main').on('click', 'input[off]', function(e) {
    prepareAnswer($('input[off], button'), 1250);
    if($(this).prop('checked')) {
        sock.send($(this).attr('on'));
    } else {
        sock.send($(this).attr('off'));
    }
})