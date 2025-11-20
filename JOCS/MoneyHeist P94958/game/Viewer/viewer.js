
// *********************************************************************
// Global variables
// *********************************************************************

// Viewer state.
var gamePaused = true;
var gamePreview = false; // If true, render will be called for the next tick even if game is paused, and then will be set to false.
var gameAnim = true;
var gameDirection = 1;
var actRound = 0; // Current round index.


// Data.
var raw_data_str; // String for storing the raw data.
var dataLoaded = false; // Set to true when raw_data_str is ready to be parsed.
var data = { } // Object for storing all the game data.


// Animation.
var speed = 10; // Ticks per second.
var FRAMES_PER_ROUND = 2;
var frames = 0; // Incremented each tick, when it reaches FRAMES_PER_ROUND, actRound is updated (acording to gameDirection).


// Visuals.
var unitSize = 0.6; // 1 = same size as tile.
var unitLineWidth = 2;
var grid_color = "#000000";
var food_color = "#ff0000";

var cell_colors = {
    '.': "#FFFFFF",
    'X': "#FFFFFF",
    'M': "#FFFFFF",
    'P': "#FFFFFF",
    'G': "#E5FF00",
    'B': "#C0C0C0",
    'H': "#FFFFFF",
    '0': "#F8A858",
    '1': "#70FF70",
    '2': "#60A8FF",
    '3': "#C0A0FF",
    'W': "#46466D",
}


var player_colors = {
    0: "#B06020",
    1: "#008000",
    2: "#0050BB",
    3: "#8000A0"
}


// *********************************************************************
// Utility functions
// *********************************************************************

function getURLParameter (name) {
    // http://stackoverflow.com/questions/1403888/get-url-parameter-with-jquery
    var a = (RegExp(name + '=' + '(.+?)(&|$)').exec(location.search)||[,null])[1]
    if (a != null) return decodeURI(a);
    return null;
}


// Callback has a single parameter with the file contents.
function loadFile (file, callback) {
    var xmlhttp;

    if (file == null || file == "") {
	alert("You must specify a file to load.");
	return;
    }

    if (window.XMLHttpRequest) xmlhttp = new XMLHttpRequest(); // Code for IE7+, Firefox, Chrome, Opera, Safari.
    else xmlhttp = new ActiveXObject("Microsoft.XMLHTTP"); // Code for IE6, IE5.

    // http://www.w3schools.com/ajax/ajax_xmlhttprequest_onreadystatechange.asp
    xmlhttp.onreadystatechange = function() {
	// Note: We cannot check xmlhttp.status != 200 for errors because status is not set when loading local files.
	if (xmlhttp.readyState == 4) callback(xmlhttp.responseText);
    }

    xmlhttp.open("GET", file, false);
    xmlhttp.send();
}


function int (s) {
    return parseInt(s);
}


function double (s) {
    return parseFloat(s);
}


function parse_assert (read_value, expected_value) {
    var correct = (read_value == expected_value);
    if (!correct) alert("Error parsing file, expected token: " + expected_value + ", read token: " + read_value + ".");
    return correct;
}


// *********************************************************************
// Initialization functions
// *********************************************************************

function parseData (raw_data_str) {
    if (raw_data_str == "") {
	alert("Could not load game file.");
	return false;
    }

    console.log("Start");

    // Convert text to tokens.
    var st = raw_data_str + "";
    var t = st.replace('\n', ' ').split(/\s+/);
    var p = 0;
    
    data.secgame = (t[p++] == "SecGame");
    
    parse_assert(t[p++], "Seed");
    data.seed = int(t[p++]);
    
    // Game and version.
    if (t[p++] != "MoneyHeist") {
	alert("Are you sure this is a MoneyHeist game file?");
	document.getElementById('file').value = "";
	document.getElementById('inputdiv').style.display = "";
	document.getElementById('loadingdiv').style.display = "none";
	return false;
    }

    data.version = t[p++];
    if (data.version != "1.0") alert("Unsupported game version! Trying to load it anyway.");
    
    parse_assert(t[p++], "NUM_PLAYERS");
    data.nb_players = int(t[p++]);
    
    parse_assert(t[p++], "NUM_ROUNDS");
    data.nb_rounds = int(t[p++]);
    
    parse_assert(t[p++], "BOARD_ROWS");
    data.rows = int(t[p++]);
    
    parse_assert(t[p++], "BOARD_COLS");
    data.cols = int(t[p++]);

    parse_assert(t[p++], "INITIAL_SOLDIERS_PER_CLAN");
    data.ini_soldiers_clan = int(t[p++]);

    parse_assert(t[p++], "INITIAL_SOLDIERS_POISONS");
    data.ini_soldiers_poison = int(t[p++]);

    parse_assert(t[p++], "SOLDIER_MAX_POISONS");
    data.soldier_max_poison = int(t[p++]);

    parse_assert(t[p++], "POINTS_FOR_KILLING_SOLDIER");
    data.pts_for_killing_soldier = int(t[p++]);

    parse_assert(t[p++], "PROFESSOR_MAX_HEALTH");
    data.professor_max_health = int(t[p++]);

    parse_assert(t[p++], "INITIAL_PROFESSOR_HEALTH");
    data.professor_ini_health = int(t[p++]);

    parse_assert(t[p++], "PROFESSOR_HEALTH_DECREASE");
    data.professor_health_decrease = int(t[p++]);

    parse_assert(t[p++], "POINTS_FOR_KILLING_PROFESSOR");
    data.pts_for_killing_professor = int(t[p++]);

    parse_assert(t[p++], "PROFESSOR_ROUNDS_REBIRTH");
    data.professor_rounds_rebirth = int(t[p++]);

    parse_assert(t[p++], "NUM_MONEY");
    data.nb_money = int(t[p++]);

    parse_assert(t[p++], "MIN_MONEY_VALUE");
    data.min_money_value = int(t[p++]);

    parse_assert(t[p++], "MAX_MONEY_VALUE");
    data.max_money_value = int(t[p++]);

    parse_assert(t[p++], "NUM_HEALTH_KITS");
    data.nb_health_kits = int(t[p++]);

    parse_assert(t[p++], "HEALTH_KIT_VALUE");
    data.health_kit_value = int(t[p++]);

    parse_assert(t[p++], "NUM_POISON_KITS");
    data.nb_poison_kits = int(t[p++]);

    parse_assert(t[p++], "POISON_KIT_VALUE");
    data.poison_kit_value = int(t[p++]);

    parse_assert(t[p++], "NUM_SAFETY_BOXES");
    data.nb_safety_boxes = int(t[p++]);

    parse_assert(t[p++], "GOLD_VALUE");
    data.gold_value = int(t[p++]);

    parse_assert(t[p++], "SIZE_HINTS_PER_BOX");
    data.size_hints_per_box = int(t[p++]);

    parse_assert(t[p++], "names");
    data.names = new Array();
    for (var i = 0; i < data.nb_players; ++i) data.names[i] = t[p++];
    
    data.rounds = new Array();
    for (var round = 0; round <= data.nb_rounds; ++round) {
	// Grid.
	
	p++; // 1st line of column labels.
        p++; // 2nd line of column labels.
	
	data.rounds[round] = new Object();
	data.rounds[round].rows = new Array();
	for (var i = 0; i < data.rows; ++i) {
	    parse_assert(t[p++], i);              // Row label.
	    data.rounds[round].rows[i] = t[p++];  // Row.
	}

	console.log("Grid done");
	
	// Units.
	parse_assert(t[p++], "units");
	data.rounds[round].units = new Array();
	data.rounds[round].units.length = int(t[p++]);
	data.rounds[round].soldiers_per_clan = new Array();
	data.rounds[round].soldiers_per_clan.length = 4;
	data.rounds[round].soldiers_per_clan[0] = 0;
	data.rounds[round].soldiers_per_clan[1] = 0;
	data.rounds[round].soldiers_per_clan[2] = 0;
	data.rounds[round].soldiers_per_clan[3] = 0;
	
	parse_assert(t[p++], "type"); 
	parse_assert(t[p++], "id"); 
	parse_assert(t[p++], "player");
	parse_assert(t[p++], "row");
	parse_assert(t[p++], "column");
	parse_assert(t[p++], "health");
	parse_assert(t[p++], "poison");
	parse_assert(t[p++], "rounds");
	for (var i = 0; i < data.rounds[round].units.length; ++i) {
	    data.rounds[round].units[i]        = new Object();
	    data.rounds[round].units[i].type   = t[p++]; // (s)oldier, (p)rofessor
	    data.rounds[round].units[i].id     = int(t[p++]);
	    data.rounds[round].units[i].player = int(t[p++]);
	    band = data.rounds[round].units[i].player;
	    console.log("Found " + data.rounds[round].units[i].type);
	    if (data.rounds[round].units[i].type == "s"){
		data.rounds[round].soldiers_per_clan[band] = data.rounds[round].soldiers_per_clan[band] + 1;
		console.log("Compto pq trobo banda " + band + " i ara val " + data.rounds[round].soldiers_per_clan[band]);		
	    }

	    data.rounds[round].units[i].i      = int(t[p++]);
	    data.rounds[round].units[i].j      = int(t[p++]);
	    data.rounds[round].units[i].health = int(t[p++]);
	    data.rounds[round].units[i].poison = int(t[p++]);
	    data.rounds[round].units[i].rounds = int(t[p++]);
	}

	console.log("Units done");
	
	// Money.
	parse_assert(t[p++], "money");
	data.rounds[round].money = new Array();
	data.rounds[round].money.length = int(t[p++]);
	parse_assert(t[p++], "row");
	parse_assert(t[p++], "column");
	parse_assert(t[p++], "amount");
	for (var i = 0; i < data.rounds[round].money.length; ++i) {
	    data.rounds[round].money[i]        = new Object();
	    data.rounds[round].money[i].i      = int(t[p++]);
	    data.rounds[round].money[i].j      = int(t[p++]);
	    data.rounds[round].money[i].amount = int(t[p++]);
	}

	console.log("Money done");
	
	// Safety box hints.
	parse_assert(t[p++], "safety_box_hints");
	data.rounds[round].hints = new Array();
	data.rounds[round].hints.length = int(t[p++]);
	for (var i = 0; i < data.rounds[round].hints.length; ++i) {
	    data.rounds[round].hints[i] = new Object();
	    data.rounds[round].hints[i].components = new Array();
	    data.rounds[round].hints[i].components.length = int(t[p++]);
	    data.rounds[round].hints[i].i = int(t[p++]);
	    data.rounds[round].hints[i].j = int(t[p++]);
	    for (var j = 0; j < data.rounds[round].hints[i].components.length; ++j) {
		data.rounds[round].hints[i].components[j] = int(t[p++]);
	    }
	}

	console.log("Hints done");

	// Round.
	parse_assert(t[p++], "round");
	if (int(t[p++]) != round) alert("Wrong round number!");

	console.log("Round done have read " + round);
	
	// Score.
	parse_assert(t[p++], "score");
	data.rounds[round].score = new Array();
	for (var i = 0; i < data.nb_players; ++i) {
	    data.rounds[round].score[i] = int(t[p++]);
	    if (data.rounds[round].score[i] > data.max_score)
		data.max_score = data.rounds[round].score[i];	    
	}

	console.log("Score done");

	// Health.
	parse_assert(t[p++], "health");
	data.rounds[round].health = new Array();
	for (var i = 0; i < data.nb_players; ++i) {
	    data.rounds[round].health[i] = int(t[p++]);
	}

	console.log("Health done");
	
	// Poison.
	parse_assert(t[p++], "poison");
	data.rounds[round].poison = new Array();
	for (var i = 0; i < data.nb_players; ++i) {
	    data.rounds[round].poison[i] = int(t[p++]);
	}

	console.log("Poison done");
	
	// Status.
	parse_assert(t[p++], "status");
	data.rounds[round].cpu = new Array();
	for (var i = 0; i < data.nb_players; ++i) {
	    var cpu = int(double(t[p++])*100);
	    data.rounds[round].cpu[i] = (cpu == -100) ? "out" : cpu + "%";
	}

	console.log("Status done");
	
	// Commands.
	if (round != data.nb_rounds) {	    
	    parse_assert(t[p++], "commands");
	    data.rounds[round].commands = new Array();
	    data.rounds[round].commands.length = int(t[p++]);
	    for (var i = 0; i < data.rounds[round].commands.length; ++i) {
		data.rounds[round].commands[i] = new Object();
		data.rounds[round].commands[i].id        = int(t[p++]);
		data.rounds[round].commands[i].action    = t[p++]; // (m)ove, (h)eal, (c)harge, (o)pen
		if (t[p-1] == "m")
		    data.rounds[round].commands[i].direction = t[p++]; // (u)p, (d)own, (r)ight, (l)eft and diagonals
		else if (t[p-1] == "h") {
		}
		else if (t[p-1] == "c") {
		}
		else {
		    parse_assert(t[p-1], "o");
		    data.rounds[round].commands[i].direction = t[p++]; // (u)p, (d)own, (r)ight, (l)eft and diagonals
		    data.rounds[round].commands[i].sol = new Array();		    
		    var size = int(t[p++]);
		    data.rounds[round].commands[i].length = size;
		    for (var j = 0; j < size; ++j) {
			data.rounds[round].commands[i].sol[j] = t[p++];
		    }
		}
	    }
	}

	console.log("Commands done");
	console.log("round " + round + " finished.");		    
    }
    console.log("Read all file")
    return true;
}


// Initializing the game.
function initGame (raw_data) {
    document.getElementById("loadingdiv").style.display = "";


    // TODO: Next two calls could run concurrently.
    if (parseData(raw_data) === false) return;


    preloadImages();


    gamePaused = false;
    gamePreview = true;

    // Canvas element.
    canvas = document.getElementById('myCanvas');
    context = canvas.getContext("2d");

    // Prepare the slider.
    $("#slider").slider({
	slide: function(event, ui) {
	    var value = $("#slider").slider( "option", "value" );
	    actRound = value;
	    frames = 0;
	    gamePaused = true;
	    gamePreview = true;
	}
    });
    $("#slider").width(600);
    $("#slider").slider("option", {min: 0, max: data.nb_rounds});

    // Set the listerners for interaction.
    document.addEventListener('mousewheel', onDocumentMouseWheel, false);
    document.addEventListener('keydown', onDocumentKeyDown, false);
    document.addEventListener('keyup', onDocumentKeyUp, false);

    window.addEventListener('resize', onWindowResize, false);
    onWindowResize();

    document.getElementById("loadingdiv").style.display = "none";
    document.getElementById("gamediv").style.display = "";

    mainloop();
}


function preloadImages () {
    data.img = new Array();
}


// *********************************************************************
// Main loop functions
// *********************************************************************

function updateGame () {
    $("#slider").slider("option", "value", actRound);
}


function writeGameState () {
    // Write round.
    $("#round").html("Round: " + actRound);

    // Update scoreboard.
    var scoreboard = "";
    for (var i = 0; i < data.nb_players; ++i) {
	scoreboard += "<span class='score'>"
	    + "<div style='display:inline-block; margin-top: 5px; width:20px; height:20px; background-color:" + cell_colors[i] + "'></div>"
	    + "<div style='display:inline-block; vertical-align: middle; margin-bottom: 7px; margin-left:8px;'>" + data.names[i] + "</div>"
	    + "<br/>"
	    + "<div style='margin-left: 10px;'>"
//	    + "<div style='padding:2px; width:200px'>Strength:  " + data.rounds[actRound].strength[i] + "</div>"
	    + "<div style='padding:2px; width:200px'>Score:  " + data.rounds[actRound].score[i] + "</div>"
	    + "<div style='padding:2px; width:200px'>Health:  " + data.rounds[actRound].health[i] + "</div>"
	    + "<div style='padding:2px; width:200px'>Poison:  " + data.rounds[actRound].poison[i] + "</div>"
	    + "<div style='padding:2px; width:200px'>Soldiers: " + data.rounds[actRound].soldiers_per_clan[i] + "</div>"
	// + "<div style='padding:2px;'>Treasures: " + data.rounds[actRound].nb_treasures[i] + "</div>"
	    // + "<div style='padding:2px;'>Dwarves:   " + data.rounds[actRound].alive_dwarves[i] + "</div>"
	    // + "<div style='padding:2px;'>Wizards:   " + data.rounds[actRound].alive_wizards[i] + "</div>"
	    + (data.secgame ? "<div style='padding:2px;'>CPU: " + data.rounds[actRound].cpu[i] + "</div>" : "")
	    + "</div>"
	    + "</span><br/><br/>";
    }
    $("#scores").html(scoreboard);

    var order = [0, 1, 2, 3];
    for (var i = 0; i < 3; ++i) {
	for (var j = i + 1; j < 4; ++j) {
	    if (data.rounds[actRound].score[order[i]] < data.rounds[actRound].score[order[j]]) {
		var k = order[i];
		order[i] = order[j];
		order[j] = k;
	    }
	}
    }

    var totalboard = "";
    for (var i = 0; i < data.nb_players; ++i) {
	totalboard += "<span class='total'>"
	    //+ data.names[order[i]] + " " + " " + " " + " " + " " 
	    + "<div style='display:inline-block; margin-top: 5px; width:20px; height:20px; background-color:" + cell_colors[order[i]] + "'></div>"
	    + "<div style='display:inline-block; vertical-align: middle; margin-bottom: 7px; margin-left:8px;'>"
	    + data.rounds[actRound].score[order[i]] + "</div>"
	    + "</span><br/><br/>";
    }
    $("#totals").html(totalboard);
}


function drawGame () {
    // Boundary check.
    if (actRound < 0) actRound = 0;
    if (actRound >= data.nb_rounds) actRound = data.nb_rounds;

    // Outter Rectangle.
    context.fillStyle = grid_color;
    context.fillRect(0, 0, tileSize*data.cols, tileSize*data.rows);

    // Draw maze.
    var meitat = tileSize/2;
    var quart = tileSize/4;
    var amplada = tileSize - 0.5;
    var rows = data.rounds[actRound].rows;
    for (var i = 0; i < data.rows; ++i) {
	var row = rows[i];
	for (var j = 0; j < data.cols; ++j) {
	    var cell = row[j];
	    var ii = i*tileSize;
	    var jj = j*tileSize;

	    if (cell == 'M') {
		drawMoney(i,j);
	    }
	    else if (cell == 'G') {
		drawGold(i,j);
	    }
	    else if (cell == 'P') {
		drawPoison(i,j);
	    }
	    else if (cell == 'H') {
		drawHealth(i,j);
	    }
	    else { // 0,1,2,3,.,W,X,B
		context.fillStyle = cell_colors[cell];
		context.fillRect(jj, ii, amplada, amplada);
	    }

	    

	    
	    // if (cell == '.'){
	    // 	context.fillStyle = cell_colors[cell];
	    // 	context.fillRect(jj, ii, amplada, amplada);
	    // }
	    // else if (cell == 'A') {
	    // 	var center_i = ii + meitat;
	    // 	var center_j = jj + meitat;
	    // 	var my_gradient = context.createRadialGradient(center_j, center_i, 0, center_j, center_i, meitat + 5);
	    // 	my_gradient.addColorStop(0, "red");
	    // 	my_gradient.addColorStop(1, "black");
	    // 	context.fillStyle = my_gradient;
	    // 	context.fillRect(jj, ii, tileSize, tileSize);
	    // }
	    // else if (cell == 'G') {
	    // 	context.strokeStyle = "#000000";
	    // 	context.fillStyle = "#000000";
	    // 	context.fillRect(jj, ii, tileSize, tileSize);
	    // 	var my_gradient = context.createLinearGradient(jj, ii + tileSize, jj + tileSize, ii);
	    // 	my_gradient.addColorStop(0, cell_colors[cell]);
	    // 	my_gradient.addColorStop(1, "#D0D0D0");
	    // 	context.fillStyle = my_gradient;
	    // 	context.fillRect(jj, ii, tileSize, tileSize);
	    // 	context.fillStyle = "#000000";
	    // 	context.strokeRect(jj + 0.5, ii + 0.5, tileSize - 1.5, tileSize - 1.5);
	    // }
	    // else if (cell >= 'a' && cell <= 'd') {
	    // 	var my_gradient = context.createLinearGradient(jj, ii, jj + tileSize, ii + tileSize);
	    // 	my_gradient.addColorStop(0, cell_colors[cell]);
	    // 	my_gradient.addColorStop(1, "#905018");
	    // 	context.fillStyle = my_gradient;
	    // 	context.fillRect(jj, ii, tileSize, tileSize);
	    // }
	    // else if (cell == 'T') {
	    // 	context.fillStyle = cell_colors['C'];
	    // 	context.fillRect(jj, ii, amplada, amplada);
	    // 	context.fillStyle = cell_colors[cell];
	    // 	var size = unitSize * tileSize * 1.2;
	    // 	var offset = (tileSize - size) / 2;
	    // 	context.beginPath();
	    // 	context.arc(jj + size/2 + offset, ii + size/2 + offset, size/2, 0, Math.PI*2, false);
	    // 	context.fill();
	    // 	context.stroke();
	    // }
	    // else {
	    // 	context.fillStyle = cell_colors[cell];
	    // 	context.fillRect(jj, ii, amplada, amplada);
	    // }
	}
    }

    
    // Draw units (here only professors).
    context.lineWidth = unitLineWidth;
    var units = data.rounds[actRound].units;
    for (var un in units) {
	var u = units[un];
	if (u.player == -1) {
	    context.strokeStyle = "#000000";
	    context.fillStyle = "#FF0000";
	}
	else {
	    context.strokeStyle = player_colors[u.player];
	    context.fillStyle = player_colors[u.player];
	}
	var i = u.i;
	var j = u.j;

	if (gameAnim) {
	    if (frames >= FRAMES_PER_ROUND/2) {
		if (u.move == 'b') i += 0.5;
		else if (u.move == 'w') { i += 0.5; j += 0.5; }
		else if (u.move == 'r') j += 0.5;
		else if (u.move == 'x') { i -= 0.5; j += 0.5; }
		else if (u.move == 't') i -= 0.5;
		else if (u.move == 'y') { i -= 0.5; j -= 0.5; }
		else if (u.move == 'l') j -= 0.5;
		else if (u.move == 'z') { i += 0.5; j -= 0.5; }
	    }
	}

	if (u.type == 'p' && u.rounds == 0) drawProfessor(i,j,player_colors[u.player]);
    }
}


function drawPoison (i, j) {
    context.fillStyle = cell_colors['M'];
    var amplada = tileSize - 0.5;
    context.fillRect(j*tileSize, i*tileSize, amplada, amplada);
    context.strokeStyle = "#000000";
    context.fillStyle = "#FF0000"; // vermell
    var size = unitSize * tileSize * 0.7;
    var offset = (tileSize - size) / 2;
    context.beginPath();
    context.arc(j*tileSize + size/2 + offset, i*tileSize + size/2 + offset, size/2, 0, 2*Math.PI, false);
    context.fill();
    context.stroke();
}

function drawHealth (i, j) {
    context.fillStyle = cell_colors['H'];
    var amplada = tileSize - 0.5;
    context.fillRect(j*tileSize, i*tileSize, amplada, amplada);
    context.strokeStyle = "#000000";
    context.fillStyle = "#0080FF"; // blau
    var size = unitSize * tileSize * 0.7;
    var offset = (tileSize - size) / 2;
    context.beginPath();
    context.arc(j*tileSize + size/2 + offset, i*tileSize + size/2 + offset, size/2, 0, 2*Math.PI, false);
    context.fill();
    context.stroke();
}

function drawMoney (i, j) {
    var size = unitSize * tileSize * 0.5;
    var offset = (tileSize - size) / 2;
    context.strokeStyle = "#E5FF00";
    context.fillStyle = "#000000"; // negre
    context.beginPath();
    context.arc(j*tileSize + size/2 + offset, i*tileSize + size/2 + offset, size/2, 0, 2*Math.PI, false);
    context.fill();
    context.stroke();
    context.beginPath();
    context.moveTo(j*tileSize + offset - 0.4*size, i*tileSize + offset - 0.4*size);
    context.lineTo(j*tileSize + offset + 1.4*size, i*tileSize + offset + 1.4*size);
    context.stroke();
    context.beginPath();
    context.moveTo(j*tileSize + offset + 1.4*size, i*tileSize + offset - 0.4*size);
    context.lineTo(j*tileSize + offset - 0.4*size, i*tileSize + offset + 1.4*size);
    context.stroke();
}

function drawGold (i, j) {
    context.fillStyle = cell_colors['G'];
    var amplada = tileSize - 0.5;
    context.fillRect(j*tileSize, i*tileSize, amplada, amplada);
}

function drawBoxDoor (i, j) {
    context.fillStyle = cell_colors['B'];
    var amplada = tileSize - 0.5;
    context.fillRect(j*tileSize, i*tileSize, amplada, amplada);
}

function drawProfessor (i, j, color) {
    context.fillStyle = color;
    context.strokeStyle = "#000000";
    var inc = 2;
    var amplada = tileSize - inc;
    context.fillRect(j*tileSize + inc, i*tileSize + inc, amplada - inc, amplada - inc);
    context.strokeRect(j*tileSize + inc, i*tileSize + inc, amplada - inc, amplada - inc);
}

// *********************************************************************
// Button events
// *********************************************************************

function playButton () {
    gamePaused = false;
}

function pauseButton () {
    gamePaused = true;
    gamePreview = true; // To call render again.
    frames = 0;
}


function startButton () {
    gamePaused = true;
    gamePreview = true;
    frames = 0;
    actRound = 0;
}


function endButton () {
    gamePreview = true;
    frames = 0;
    actRound = data.nb_rounds;
}


function animButton () {
    gameAnim = !gameAnim;
}


function closeButton () {
    window.close();
}


// *********************************************************************
// Keyboard and Mouse events
// *********************************************************************

function onDocumentMouseWheel (event) {
}


function onDocumentKeyDown (event) {
}


function onDocumentKeyUp (event) {
    // http://www.webonweboff.com/tips/js/event_key_codes.aspx
    switch (event.keyCode) {
    case 36: // Start.
	gamePreview = true;
	actRound = 0;
	frames = 0;
	break;

    case 35: // End.
	gamePreview = true;
	actRound = data.nb_rounds;
	frames = 0;
	break;

    case 33: // PageDown.
	gamePreview = true;
	actRound -= 10;
	frames = 0;
	break;

    case 34: // PageUp.
	gamePreview = true;
	actRound += 10;
	frames = 0;
	break;

    case 38: // ArrowUp.
    case 37: // ArrowLeft.
	gamePaused= true;
	gamePreview = true;
	--actRound;
	frames = 0;
	break;

    case 40: // ArrowDown.
    case 39: // ArrowRight.
	gamePaused = true;
	gamePreview = true;
	++actRound;
	frames = 0;
	break;

    case 32: // Space.
	if (gamePaused) playButton();
	else pauseButton();
	break;

    case 72: // "h"
	help();
	break;

    default:
	// $("#debug").html(event.keyCode);
	break;
    }
}


function onWindowResize (event) {
    // Constants.
    var header_height = 150;
    var canvas_margin = 20;

    // Set canvas size.
    var size = Math.min(document.body.offsetWidth, document.body.offsetHeight - header_height) - canvas_margin*2;

    canvas.width  = size;
    canvas.height = size;

    var max_dimension = Math.max(data.cols,data.rows);
    tileSize = size / max_dimension;

    drawGame();
}


function help () {
    // Opens a new popup with the help page.
    var win = window.open('help.html', 'name', 'height=400, width=300');
    if (window.focus) win.focus();
    return false;
}


// *********************************************************************
// This function is called periodically.
// *********************************************************************

function mainloop () {
    // Configure buttons.
    if (gamePaused) {
	$("#but_play").show();
	$("#but_pause").hide();
    }
    else {
	$("#but_play").hide();
	$("#but_pause").show();
    }

    if (actRound < 0) actRound = 0;

    if (actRound > data.nb_rounds) {
	actRound = data.nb_rounds;
	gamePaused = true;
	frames = 0;
    }

    if (!gamePaused || gamePreview) {
	updateGame();
	drawGame();
	writeGameState();

	if (gamePreview) {
	    frames = 0;
	    gamePreview = false;
	}
	else {
	    ++frames;
	    if (frames == FRAMES_PER_ROUND) {
		frames = 0;
		actRound += gameDirection;
	    }
	}
    }

    // Periodically call mainloop.
    var frame_time = 1000/speed;
    setTimeout(mainloop, frame_time);
}


// *********************************************************************
// Main function, it is called when the document is ready.
// *********************************************************************

function init () {
    console.log("Init");
    // Get url parameters.
    var game;
    if (getURLParameter("sub") != null) {
	var domain = window.location.protocol + "//" + window.location.host;
	if (getURLParameter("nbr") != null)
	    game = domain + "/?cmd=lliuraments&sub=" + getURLParameter("sub") + "&nbr=" + getURLParameter("nbr") + "&download=partida";
	else
	    game = domain + "/?cmd=partida&sub=" + getURLParameter("sub") + "&download=partida";
    }
    else game = getURLParameter("game");

    if (game == null || game == "") {
	// Ask the user for a game input.
	var inputdiv = document.getElementById('inputdiv')
	inputdiv.style.display = "";
	document.getElementById('file').addEventListener('change', function(evt) {
	    //http://www.html5rocks.com/en/tutorials/file/dndfiles/
	    var file = evt.target.files[0];
	    var reader = new FileReader();
	    reader.readAsText(file);
	    reader.onloadend = function(evt) {
		if (evt.target.readyState != FileReader.DONE) alert("Error accessing file.");
		else { // DONE == 2.
		    inputdiv.style.display = "none";
		    document.getElementById("loadingdiv").style.display = "";
		    initGame(reader.result);
		}
	    };
	}, false);
    }
    else {
	document.getElementById("loadingdiv").style.display = "";
	// Load the given game.
	loadFile(game, initGame);
    }
}
