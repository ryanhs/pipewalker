// ================================================================================
/*
 * do it
 */
testAlgoAStarIterator = 0;
testAlgoAStarTimeout = 0;
testAlgoAStarTimeoutTimer = 4;
var testAlgoAStarStart;
function testAlgoAStar(){
	
	testAlgoAStarTimeout = false;
	testAlgoAStarIterator = 0;
	
	startSolution = AI_getCurrentSolution();
	startStringify = JSON.stringify(startSolution);
	testAlgoAStarStart = board.evaluator();
	
	closedSet = [];
	openSet = [startStringify];
	
	cameFrom = {};
	
	gScore = {};
	gScore[startStringify] = 0;
	
	fScore = {};
	fScore[startStringify] = Math.abs(board.evaluatorCache.result - 1000); // use -1000 to replicate heuristic_cost_estimate()
	
	function do_AStar(){
		var idx;
		
		if(openSet.length == 0){
			$('.AStar-stop-btn').addClass('hide');
			$('.AStar-btn').removeAttr('disabled');
			console.log('no solution!'); // failure
			return;
		}
		
		openSet.sort(function(a, b){
			if(fScore[a] > fScore[b]) return 1;
			if(fScore[a] < fScore[b]) return -1;
			return 0;
		});
		
		currentStringify = openSet[0];
		currentSolution = JSON.parse(openSet[0]);
		current = AI_evaluator(currentSolution);
		
		//console.log(currentSolution);
		
		// debug
		testAlgoAStarIterator++;
		logStr = ('[A*] i=' + testAlgoAStarIterator + ' -> m=' + (fScore[currentStringify]).toFixed(2));
		console.log(logStr);
		loger.write(logStr);
		
		// put in real action
		board.data = current.board.data;
		board.reconnectAll();
		//if(testAlgoAStarIterator % 10 == 0 || board.isAllClientOK())
			refreshBoard();
		
		// if finish, do animation
		if(current.board.isAllClientOK()) return testAlgoAStarReconstructPath(cameFrom, currentStringify);
		
		openSet.splice(0, 1);
		closedSet.push(currentStringify);
		
		// list neighbors
		var neighbors = [];
		var neighborsEvaluator = {};
		var row, cell, neighborSolution, neighborSolutionStringify;
		var possibleDirection = ['up', 'right', 'down', 'left'];
		for(row = 0; row < boardSize; row++){
			for(cell = 0; cell < boardSize; cell++){
				
				if(currentSolution[row][cell] != false){
					for(idx in possibleDirection){
						neighborSolution = $.extend(true, [], currentSolution); // jQuery hack
						neighborSolution[row][cell] = possibleDirection[idx];
						neighborSolutionStringify = JSON.stringify(neighborSolution);
						
						neighbors.push(neighborSolutionStringify);
						neighborsEvaluator[neighborSolutionStringify] = AI_evaluator(neighborSolution);
						
					}
				}
				
			}
		}
		
		// suffle first, to make more awesome!
		neighbors.sort(function(a, b){ return 5 - mt_rand(10); })
		
		// process neighbors
		for(idx in neighbors){
			if(closedSet.indexOf(neighbors[idx]) != -1) continue;
			
			
			var tentative_gScore = gScore[currentStringify] + 1; // dist between is 1, on block at the time
			if(openSet.indexOf(neighbors[idx]) == -1){
				openSet.push(neighbors[idx]);
			}else if(tentative_gScore >= gScore[neighbors[idx]]){
				continue; // this is not better path
			}
			
			cameFrom[neighbors[idx]] = currentStringify;
			gScore[neighbors[idx]] = tentative_gScore;
			fScore[neighbors[idx]] = tentative_gScore + Math.abs(neighborsEvaluator[neighbors[idx]].result - 1000)
		}
		
		testAlgoAStarIterator++;
		testAlgoAStarTimeout = setTimeout(do_AStar, 1);
	}
	
	testAlgoAStarTimeout = setTimeout(do_AStar, 1);
}


function testAlgoAStarReconstructPath(cameFrom, currentStringify){
	logStr = ('[A*] recontruct path for animation!');
	console.log(logStr);
	loger.write(logStr);
	
	totalPath = [currentStringify];
	
	while(cameFrom[currentStringify] !== undefined){
		currentStringify = cameFrom[currentStringify];
		totalPath.push(currentStringify);
	}
	
	
	pathReversed = [];
	for(i = totalPath.length; i > 0; i--){
		pathReversed.push(totalPath[i - 1])
	}
	
	prevStringify = pathReversed[0];
	function do_animate(){
		if(pathReversed.length == 0){
			setTimeout(function(){
				$('.AStar-stop-btn').click();
				$('.alert-win').modal();
			}, 10);
			return;
		}
		
		nowStringify = pathReversed[0];
		nowSolution = JSON.parse(nowStringify);
		now = AI_evaluator(nowSolution);
		
		pathReversed.splice(0, 1);
		
		// put in real action
		board.data = now.board.data;
		board.reconnectAll();
		refreshBoard();
		
		prevStringify = nowStringify;
		testAlgoAStarTimeout = setTimeout(do_animate, 500 / totalPath.length);
	}
	
	board = testAlgoAStarStart.board;
	board.reconnectAll();
	refreshBoard();
	logStr = ('[A*] do animation!');
	console.log(logStr);
	loger.write(logStr);
	
	testAlgoAStarTimeout = setTimeout(do_animate, 500 / totalPath.length);
}
