var timeInSecs;
var ticker;
var elem = "continue";

// src: http://www.codingforums.com/javascript-programming/269745-javascript-countdown-disabling-submit-button.html

function startTimer(secs)
{
	// disable element
	document.getElementById(elem).disabled = true;
	// Set text
	document.getElementById(elem).innerHTML = "Bitte warten(5)";
	timeInSecs = parseInt(secs);
	tick();  // start timer right away
	ticker = setInterval("tick()",1000);   // every second
}

document.onkeydown = function(e)
{
    e = e || window.event;
    if (e.keyCode == 13)
    {
        startGame();
    }
};

function startGame()
{
	if (timeInSecs <= 0)
		simpleRacer.onStartNewGame();
}

function tick()
{
	var secs = timeInSecs;
	if (secs > 0)
	{
		timeInSecs--;
	}
	else
	{
		clearInterval(ticker); // stop counting at zero
		document.getElementById(elem).disabled = false;
		document.getElementById(elem).innerHTML = "Bitte folgende Taste drücken, um zu spielen:";
		document.getElementById("img").style.visibility = "visible";
		return;
	}
	document.getElementById(elem).innerHTML = "Bitte warten("+secs+")";
}