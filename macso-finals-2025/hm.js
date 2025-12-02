// SIMULATOR CONTENT
const NUM_PLAYERS = 6;
const PLAYER_ARRAY = [0, 1, 2, 3, 4, 5];
var tournamentCount = 0;
var tourState = [[-1, 1, 1, 1, 1, 1],
				 [0, -1, 1, 1, 1, 1],
				 [0, 0, -1, 1, 1, 1],
				 [0, 0, 0, -1, 1, 1],
				 [0, 0, 0, 0, -1, 1],
				 [0, 0, 0, 0, 0, -1]];
var previousTour;
var totalWins = [0, 0, 0, 0, 0, 0];

// Reset switches.
for (var u = 0; u < NUM_PLAYERS; u++) {
	for (var v = 0; v < u; v++) {
		showWin(v, u);
	}
}


// Copeland set.
function CL_J() {
	var m = 0;
	var ms = [];
	for (var i = 0; i < NUM_PLAYERS; i++) {
		var ct = tourState[i].filter(x => x == 1).length;
		if (ct > m) {
			m = ct;
			ms = [i];
		} else if (ct == m) {
			ms.push(i);
		}
	}
	return ms;
}

// Upsetters.
function UPS_J() {
	var out = [];
	for (var u = 0; u < NUM_PLAYERS; u++) {
		for (var v = 0; v < u; v++) {
			if (tourState[u][v]) out.push(u);
		}
	}
	return [...new Set(out)];
}

// Top cycle.
function TC_J() {
	var out = [];
	for (var r = 0; r < NUM_PLAYERS; r++) {
		var vis = 1 << r;
		var s = [r];
		while (s.length) {
			var u = s.pop();
			for (var v = 0; v < NUM_PLAYERS; v++) {
				if (tourState[u][v] == 1 && (vis & (1 << v)) == 0) {
					vis |= (1 << v);
					s.push(v);
				}
			}
		}
		if (vis == (1 << NUM_PLAYERS) - 1) {
			out.push(r);
		}
	}
	return out;
}

// Beat next in cycle.
function BOD_J() {
	return PLAYER_ARRAY.filter(u => tourState[u][(u + 1) % NUM_PLAYERS]);
}

// Revengers.
function REV_J() {
	if (tournamentCount < 2) return [];
	var rvs = [];
	for (var u = 0; u < NUM_PLAYERS; u++) {
		for (var v = 0; v < u; v++) {
			if (tourState[u][v] == previousTour[v][u]) {
				if (tourState[u][v]) {
					rvs.push(u);
				} else {
					rvs.push(v);
				}
			}
		}
	}
	var out = [...new Set(rvs)];
	out.sort();
	return out;
}

// Beat team 2 (1-indexed).
function B2_J() {
	return PLAYER_ARRAY.filter(i => tourState[i][1] == 1);
}

// Modal value.
function MD_J() {
	var wa = tourState.map(r => r.filter(x => x == 1).length);
	var ma = wa.map(i => wa.filter(w => w == i).length);
	var mc = Math.max(...ma);
	return PLAYER_ARRAY.filter(i => ma[i] == mc);
}

// Seed divides #wins.
function SDW_J() {
	return PLAYER_ARRAY.filter(i => (totalWins[i] > 0) && (totalWins[i] % (i + 1) == 0));
}

// Match degree of next in cycle.
function DMD_J() {
	var wa = tourState.map(r => r.filter(x => x == 1).length);
	return PLAYER_ARRAY.filter(u => wa[u] == wa[(u + 1) % NUM_PLAYERS]);
}

// Not upset.
function NUS_J() {
	var out = new Set();
	for (var u = 0; u < NUM_PLAYERS; u++) {
		out.add(u);
	}
	for (var u = 0; u < NUM_PLAYERS; u++) {
		for (var v = 0; v < u; v++) {
			if (tourState[u][v]) out.delete(v);
		}
	}
	return [...out].sort();
}

// JUDGE ORDERING
const jf = [B2_J, CL_J, UPS_J, MD_J, BOD_J, NUS_J, DMD_J, TC_J, REV_J, SDW_J];
const [B2, CL, UPS, MD, BOD, NUS, DMD, TC, REV, SDW] = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10];
const NUM_JUDGES = jf.length;

function update(u, v) {
	tourState[u][v] = 1 - tourState[u][v];
	tourState[v][u] = 1 - tourState[v][u];
	const cell = document.getElementById("td" + u + v);
	if (cell.children[0].children[0].checked) {
		showWin(u, v);
	} else {
		showWin(v, u);
	}
}

function showWin(u, v) {
	const cell = document.getElementById("td" + u + v);
	const wlText = document.getElementById("res" + u + v);
	const oppCell = document.getElementById("td" + v + u);
	const oppText = document.getElementById("res" + v + u);
	cell.style.backgroundColor = "#cfc";
	wlText.innerHTML = "W";
	oppCell.style.backgroundColor = "#fcc";
	oppText.innerHTML = "L";
}

function recordTour() {
	previousTour = [];
	for (var r = 0; r < NUM_PLAYERS; r++) {
		previousTour.push([...tourState[r]]);
	}
}

function formatTeamString(teams) {
	if (teams.length == 0) {
		return "";
	} else if (teams.length == 1) {
		return "Team " + teams[0];
	} else if (teams.length == 2) {
		return "Teams " + teams[0] + " and " + teams[1];
	} else {
		var last = teams[teams.length - 1];
		return "Teams " + teams.filter(x => x != last).join(", ") + ", and " + last;
	}
}

function formatResultString(results) {
	strings = []
	for (var j = 0; j < NUM_JUDGES; j++) {
		var judgeNum = j + 1;
		var resultSet = results[j].map(x => x + 1);
		if (resultSet.length == 0) {
			strings.push("Judge " + judgeNum + ": Eh, I don't really think any of the teams won there.");
		} else {
			strings.push("Judge " + judgeNum + ": Looks like "
				+ formatTeamString(resultSet) + " won that tournament!");
		}
	}
	return "Simulated tournament #" + tournamentCount + " results:"
		+ "<br/><ul><li>" + strings.join("</li><li>") + "</li></ul>";
}

function displayResults(results) {
	document.getElementById("judge").innerHTML = formatResultString(results);
}

function updateStats() {
	tournamentCount++;
	for (var i = 0; i < NUM_PLAYERS; i++) {
		totalWins[i] += tourState[i].filter(x => x == 1).length;
	}
}

function runTour() {
	updateStats();
	var results = jf.map(f => f());
	recordTour();
	displayResults(results);
}

// PUZZLE CONTENT

const tableInfo = 
	[
		// 1
		[["Schneider", "W"], // N 4
		 ["Jakobsson", "SE"], // O 4
		 ["Stenersen", "NW"], // T 2
		 ["Baumann", "E"], // A 2
		 ["Olafsson", "SW"], // L 2
		 ["Larsen", "NE"]], // L 1
		// 2
		[["Mason", "NE"], // O 4
		 ["Berndt", "E"],
		 ["Brown", "W"], // W 4
		 ["Lagerback", "SE"],
		 ["Eriksen", "NW"],
		 ["Jones", "SW"]],
		// 3
		[["Bergmann", "W"],
		 ["Smith", "NE"],
		 ["Scheving", "SW"],
		 ["Andersen", "SE"], // D 3
		 ["Turner", "NW"], // R 3
		 ["Antonsson", "E"]], // A 1
		// 4
		[["Lindberg", "SE"],
		 ["Persson", "SW"],
		 ["Swensen", "W"], // W 2
		 ["Karlsson", "NE"], // A 2
		 ["Blondal", "E"], // L 2
		 ["Alrick", "NW"]], // L 2
		// 5
		[["Fjeldsted", "SW"],
		 ["Augustinsson", "SE"], // U 4
		 ["Robinson", "NE"],
		 ["Sampsted", "NW"], // P 4
		 ["Eisen", "E"], // S 3
		 ["Green", "W"]],
		// 6
		[["Martin", "E"],
		 ["Brynhildsen", "NE"],
		 ["Iversen", "SE"],
		 ["Reyes", "W"], // E 2
		 ["Davies", "NW"],
		 ["Steiner", "SW"]], // T 2
		// 7
		[["Yang", "E"],
		 ["Olsson", "NW"], // S 3
		 ["Solbakken", "NE"],
		 ["Weber", "W"],
		 ["Petersen", "SE"], // P 1
		 ["Thoresen", "SW"]],
		// 8
		[["Keller", "W"], // E 5
		 ["Lehner", "NW"],
		 ["Jensen", "SE"],
		 ["Freya", "E"],
		 ["Nyland", "NE"],
		 ["Ward", "SW"]],
		// 9
		[["Svanberg", "SE"],
		 ["Christensen", "NW"], // R 3
		 ["White", "SW"], // T 4
		 ["Schar", "NE"],
		 ["Meier", "E"],
		 ["Nordfeldt", "W"]], // O 2
		// 10
		[["Djuric", "W"], // U 3
		 ["Carlsen", "SW"], // R 3
		 ["Schram", "SE"],
		 ["Johansson", "NW"],
		 ["Karlstrom", "NE"],
		 ["Lodemann", "E"]],
	];

const puzzleInfo =
	[
		// 1
		{"name": "Igloo Nibbling", "head": B2,
		"data": [[CL, `I think only Teams 1 and 2 should win.`],
				 [UPS, `I think only Teams 3, 5, and 6 should win.`],
				 [MD, `I think only Teams 3, 4, and 5 should win.`]
				]},
		// 2
		{"name": "Damp Endurance", "head": CL,
		"data": [[BOD, `I think only Teams 1, 3, and 4 should win.`],
				 [TC, `I think only Teams 1, 2, 3, and 4 should win.`]
				]},
		// 3
		{"name": "X-mas Blizzarding", "head": UPS,
		"data": [[SDW, `I don't really mind who else wins, as long as it includes Team 5.`],
				 [MD, `I think only Teams 2, 4, and 5 should win.`],
				 [CL, `I think only Team 1 should win.`],
				 [TC, `I don't really mind who else wins, as long as it includes Team 3.`],
				 [BOD, `I don't really mind who else wins, as long as it includes Teams 4 and 6.`]
				]},
		// 4
		{"name": "Yuletide Noisemaking", "head": MD,
		"data": [[REV, `I think only Teams 2, 3, and 6 should win.`],
				 [CL, `I think only Team 1 should win.`],
				 [DMD, `I don't really mind who else wins, as long as it includes Team 3.`],
				 [B2, `I don't really mind who else wins, as long as it includes Team 6.`]
				]},
		// 5
		{"name": "Upbeat Merrymaking", "head": BOD,
		"data": [[REV, `I think only Teams 1, 2, 4, and 5 should win.`],
				 [SDW, `I think only Teams 1 and 5 should win.`],
				 [CL, `I think only Teams 2 and 4 should win.`],
				 [TC, `I don't really mind who wins, as long as exactly 4 teams win.`],
				 [B2, `I don't really mind who else wins, as long as it includes Team 5.`]
				]},
		// 6
		{"name": "Midwinter Alpining", "head": NUS,
		"data": [[BOD, `I think only Teams 2, 3, and 4 should win.`],
				 [REV, `I don't really mind who else wins, as long as Team 5 doesn't win!`],
				 [UPS, `I think only Teams 2, 3, 5, and 6 should win.`],
				 [DMD, `I don't think any of the teams should win.`]
				]},
		// 7
		{"name": "Tinsel Crafting", "head": DMD,
		"data": [[UPS, `I don't really mind who wins, as long as exactly 1 team wins.`],
				 [NUS, `I don't really mind who wins, as long as exactly 5 teams win.`],
				 [REV, `I don't really mind who else wins, as long as it includes Team 2.`]
				]},
		// 8
		{"name": "Hollyhock Eating", "head": TC,
		"data": [[DMD, `I think strictly more than 3 teams should win.`],
				 [NUS, `I think strictly more than 3 teams should win.`],
				 [SDW, `I think strictly more than 3 teams should win.`],
				 [CL, `I think strictly fewer than 3 teams should win.`],
				 [UPS, `I think strictly fewer than 3 teams should win.`]
				]},
		// 9
		{"name": "Snow Working", "head": REV,
		"data": [[DMD, `I think at least 1 team should win.`],
				 [CL, `I don't really mind who wins, as long as exactly 2 teams win.`],
				 [UPS, `I don't really mind who wins, as long as exactly 2 teams win.`],
				 [SDW, `I don't really mind who wins, as long as exactly 3 teams win.`],
				 [REV, `I don't really mind who wins, as long as exactly 3 teams win.`],
				 [B2, `My set of winners is exactly the same as Judge ${CL}'s.`],
				 [NUS, `My set of winners contains all of Judge ${UPS}'s set of winners, plus at least one more winner.`],
				 [BOD, `My set of winners contains all of Judge ${NUS}'s set of winners, plus at least one more winner.`],
				 [MD, `Between my set of winners and Judge ${REV}'s set of winners, all teams are represented at least once.`]
				]},
		// 10
		{"name": "Overnight Noel-ing", "head": SDW,
		"data": [[CL, `If we ignore Team 2 and all its matches, every team would win.`],
				 [UPS, `If we ignore Team 4 and all its matches, only teams 3 and 6 would win.`],
				 [B2, `If we ignore Team 5 and all its matches, exactly 2 teams would win.`],
				 [NUS, `If we ignore Team 6 and all its matches, only teams 3 and 5 would win.`],
				]},
	];
const NUM_PUZZLES = puzzleInfo.length;

function renderReq(req) {
	const judgePreamble = "Judge " + req[0] + ": ";
	return judgePreamble + req[1];
}

function renderTours() {
	var tourStrings = [];
	for (var puzz = 0; puzz < NUM_PUZZLES; puzz++) {
		const headerString = "<h3>Tournament #" + (puzz + 1) + ": " + puzzleInfo[puzz].name + "</h3>";
		const tableHeaderString = "<h4>Players in this tournament</h4>";
		var tableString = "<table style=\"width:60%\"><th>Team seed</th><th>Name</th><th>Starting corner</th>";
		for (var i = 0; i < NUM_PLAYERS; i++) {
			tableString += "<tr><td>" + (i+1) + "</td><td>" + tableInfo[puzz][i].join("</td><td>") + "</td><td></tr>";
		}
		tableString += "</table>";
		const reqsListString = "<ul><li>"
			+ [...puzzleInfo[puzz].data].sort((a, b) => a[0] - b[0]).map(req => renderReq(req)).join("</li><li>")
			+ "</li></ul>";
		// const headJudgeString = "<p>Head judge: Judge " + puzzleInfo[puzz].head + "</p>";
		tourStrings.push(headerString + tableString + reqsListString);
	}
	document.getElementById("tours").innerHTML = tourStrings.join("<hr>");
}

renderTours();
