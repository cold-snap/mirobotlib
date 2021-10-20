$(function(){

	// Cache some selectors

	var clock = $('#clock'),
		alarm = clock.find('.alarm'),
		ampm = clock.find('.ampm'),
		dialog = $('#alarm-dialog').parent(),
		alarm_set = $('#alarm-set'),
		alarm_clear = $('#alarm-clear'),
		time_is_up = $('#time-is-up').parent();

	// This will hold the number of seconds left
	// until the alarm should go off
	var alarm_counter = -1;

	// Map digits to their names (this will be an array)
	var digit_to_name = 'zero one two three four five six seven eight nine'.split(' ');

	// This object will hold the digit elements
	var digits = {};
    var digit = {};
	// Positions for the hours, minutes, and seconds
	var position = [
		'Y1', 'Y2', 'Y3', 'Y4', ' ', 'M1', 'M2', ' ', ' ', 'D1', 'D2'
	];
	var positions = [
		'h1', 'h2', ' ', 'm1', 'm2', ' ', 's1', 's2'
	];

	// Generate the digits with the needed markup,
	// and add them to the clock

    var digit_holder = clock.find('.digit');
	$.each(position, function(){
        if(this == '.'){
			digit_holder.append('<div class="dots">');
		}
		else{

			var pos = $('<div>');

			for(var i=1; i<8; i++){
				pos.append('<span class="d' + i + '">');
			}

			// Set the digits as key:value pairs in the digits object
			digit[this] = pos;

			// Add the digit elements to the page
			digit_holder.append(pos);
		}		
	});
	var digits_holder = clock.find('.digits');
	$.each(positions, function(){
        
		if(this == ':'){
			digits_holder.append('<div class="dots">');
		}
		else{

			var pos = $('<div>');

			for(var i=1; i<8; i++){
				pos.append('<span class="d' + i + '">');
			}

			// Set the digits as key:value pairs in the digits object
			digits[this] = pos;

			// Add the digit elements to the page
			digits_holder.append(pos);
		}

	});

	// Add the weekday names

	var weekday_names = 'MON TUE WED THU FRI SAT SUN'.split(' '),
		weekday_holder = clock.find('.weekdays');

	$.each(weekday_names, function(){
		weekday_holder.append('<span>' + this + '</span>');
	});

	var weekdays = clock.find('.weekdays span');


	// Run a timer every second and update the clock

	(function update_time(){

		// Use moment.js to output the current time as a string
		// hh is for the hours in 12-hour format,
		// mm - minutes, ss-seconds (all with leading zeroes),
		// d is for day of week and A is for AM/PM
        var date = moment().format("YYYYMMDDdA");
        digit.Y1.attr('class', digit_to_name[date[0]]);
		digit.Y2.attr('class', digit_to_name[date[1]]);
		digit.Y3.attr('class', digit_to_name[date[2]]);
		digit.Y4.attr('class', digit_to_name[date[3]]);
		digit.M1.attr('class', digit_to_name[date[4]]);
		digit.M2.attr('class', digit_to_name[date[5]]);
		digit.D1.attr('class', digit_to_name[date[6]]);
		digit.D2.attr('class', digit_to_name[date[7]]);
		
		var now = moment().format("hhmmssdA");
		digits.h1.attr('class', digit_to_name[now[0]]);
		digits.h2.attr('class', digit_to_name[now[1]]);
		digits.m1.attr('class', digit_to_name[now[2]]);
		digits.m2.attr('class', digit_to_name[now[3]]);
		digits.s1.attr('class', digit_to_name[now[4]]);
		digits.s2.attr('class', digit_to_name[now[5]]);

		// The library returns Sunday as the first day of the week.
		// Stupid, I know. Lets shift all the days one position down, 
		// and make Sunday last

		var dow = now[6];
		dow--;
		
		// Sunday!
		if(dow < 0){
			// Make it last
			dow = 6;
		}

		// Mark the active day of the week
		weekdays.removeClass('active').eq(dow).addClass('active');

		// Set the am/pm text:
		ampm.text(now[7]+now[8]);

		// Schedule this function to be run again in 1 sec
		setTimeout(update_time, 1000);

	})();

});