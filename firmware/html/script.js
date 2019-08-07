$(function() {
    // Color Picker
    $('.color').colorPicker({
        buildCallback: function($elm) {
            var colorInstance = this.color;
            var colorPicker = this;

            $elm.append('<div class="cp-memory">' +
                '<div style="background-color: #FFFFFF";></div>' +
                '<div style="background-color: #FF0000";></div>' +
                '<div style="background-color: #FFFF00";></div>' +
                '<div style="background-color: #00FF00";></div>' +
                '<div style="background-color: #00FFFF";></div>' +
                '<div style="background-color: #0000FF";></div>' +
                '<div style="background-color: #FF00FF";></div>' +
                '<div style="background-color: #000000";></div>').
            on('click', '.cp-memory div', function(e) {
                var $this = $(this);

                if (this.className) {
                    $this.parent().prepend($this.prev()).children().eq(0).
                    css('background-color', '#' + colorInstance.colors.HEX);
                } else {
                    colorInstance.setColor($this.css('background-color'));
                    colorPicker.render();
                }
            });

            this.$colorPatch = $elm.prepend('<div class="cp-disp">').find('.cp-disp');
        },

        cssAddon: '.cp-memory {margin-bottom:6px; clear:both;}' +
            '.cp-memory div {float:left; width:12.5%; height:40px;' +
            'background:rgba(0,0,0,1); text-align:center; line-height:40px;}' +
            '.cp-disp{padding:10px; margin-bottom:6px; font-size:19px; height:40px; line-height:20px}' +
            '.cp-xy-slider{width:200px; height:200px;}' +
            '.cp-xy-cursor{width:16px; height:16px; border-width:2px; margin:-8px}' +
            '.cp-z-slider{height:200px; width:40px;}' +
            '.cp-z-cursor{border-width:8px; margin-top:-8px;}',

        opacity: false,

        renderCallback: function($elm, toggled) {
            var colors = this.color.colors.RND;
            var json = {
                'r': colors.rgb.r,
                'g': colors.rgb.g,
                'b': colors.rgb.b
            };

            this.$colorPatch.css({
                backgroundColor: '#' + colors.HEX,
                color: colors.RGBLuminance > 0.22 ? '#222' : '#ddd'
            }).text(this.color.toString($elm._colorMode)); // $elm.val();

            var tmode = $('#tmode option:selected').val();
            // if (typeof effectInfo[tmode].wsTCode !== 'undefined') {
            //     if (effectInfo[tmode].hasColor) {
            //         wsEnqueue( effectInfo[tmode].wsTCode + JSON.stringify(json) );
            //     }
            // }
        }
    });
    $('ul.navbar-nav li a').click(function() {
        // Highlight proper navbar item
        $('.nav li').removeClass('active');
        $(this).parent().addClass('active');

        // Show the proper menu div
        $('.mdiv').addClass('hidden');
        $($(this).attr('href')).removeClass('hidden');

        // kick start the live stream
        if ($(this).attr('href') == "#stream") {
            // wsEnqueue('T9');
        }

        // Collapse the menu on smaller screens
        $('#navbar').removeClass('in').attr('aria-expanded', 'false');
        $('.navbar-toggle').attr('aria-expanded', 'false');

        // Firmware selection and upload
        $('#efu').change(function() {
            $('#updatefw').submit();
            $('#update').modal();
        });

        // Restore config selection and upload
        $('#restoreFile').change(function() {
            $('#restoreForm').submit();
            $('#update').modal();
        });



        // Set page event feeds
        feed();
    });
    // DHCP field toggles
    $('#dhcp').click(function() {
        if ($(this).is(':checked')) {
            $('.dhcp').addClass('hidden');
        } else {
            $('.dhcp').removeClass('hidden');
        }
    });

    // MQTT field toggles
    $('#mqtt').click(function() {
        if ($(this).is(':checked')) {
            $('.mqtt').removeClass('hidden');
        } else {
            $('.mqtt').addClass('hidden');
        }
    });
    // Pixel type toggles
    $('#p_type').change(function() {
        if ($('select[name_type]').val() == '1')
            $('#p_color').prop('disabled', true);
        else
            $('#p_color').prop('disabled', false);
    });
});

function wsConnect() {
    if ('WebSocket' in window) {

        // accept ?target=10.0.0.123 to make a WS connection to another device
        if (target = param('target')) {
            // 
        } else {
            target = document.location.host;
        }
        ws = new WebSocket('ws://' + target + '/ws');
        ws.binaryType = 'arraybuffer';
        ws.onmessage = function(event) {
            //Handle responses here
        };
    }
}

function wsSend(data) {
    ws.send(data);
}