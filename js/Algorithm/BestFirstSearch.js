// ================================================================================
/*
 * do it
 * */
testAlgoBFSlog = [];
testAlgoBFSstack = [];
testAlgoBFSIterator = 0;
testAlgoBFSTimeout = 0;
testAlgoBFSTimeoutTimer = 4;
function testAlgoBFS(){
	if(board.isAllClientOK() && testAlgoBFSIterator == 0){
		$('.BFS-stop-btn').addClass('hide');
		$('.BFS-btn').removeAttr('disabled');
		return true;
	}
	if(board.isAllClientOK() && testAlgoBFSIterator > 0){
		$('.BFS-stop-btn').click();
		$('.alert-win').modal();
		return true;
	}
	
	
	var solution = AI_getCurrentSolution();
	var moves = [];
	var movesEvaluator = {};
	
	var row, cell, newSolution;
	var possibleDirection = ['up', 'right', 'down', 'left'];
	for(row = 0; row < boardSize; row++){
		for(cell = 0; cell < boardSize; cell++){
			
			if(solution[row][cell] != false){
				for(idx in possibleDirection){
					newSolution = $.extend(true, [], solution); // jQuery hack
					newSolution[row][cell] = possibleDirection[idx];
					newSolutionStringify = JSON.stringify(newSolution);
					
					if(testAlgoBFSlog.indexOf(newSolutionStringify) != -1) continue;
					
					moves.push(newSolutionStringify);
					movesEvaluator[newSolutionStringify] = AI_evaluator(newSolution);
				}
			}
			
		}
	}
	
	if(moves.length > 0){
		moves.sort(function(a, b){
			if(movesEvaluator[a].result > movesEvaluator[b]) return 1;
			if(movesEvaluator[a].result < movesEvaluator[b]) return -1;
			return 0;
		});
		
		// debug
		testAlgoBFSIterator++;
		logStr = ('[BFS] i=' + testAlgoBFSIterator + ' -> m=' + (movesEvaluator[moves[0]].result).toFixed(2));
		console.log(logStr);
		loger.write(logStr);
		
		// put in real action
		board.data = movesEvaluator[moves[0]].board.data;
		board.reconnectAll();
		if(testAlgoBFSIterator % 10 == 0 || board.isAllClientOK())
			refreshBoard();
		
		// cache move
		testAlgoBFSlog.push(moves[0]);
		testAlgoBFSstack.push(moves[0]);
	}else{
		var stackStringify = testAlgoBFSstack.pop();
		var stack = AI_evaluator(stackStringify);
		
		// debug
		testAlgoBFSIterator++;
		logStr = ('[BFS] i=' + testAlgoBFSIterator + ' -> m=' + (stack.result).toFixed(2));
		console.log(logStr);
		loger.write(logStr);
		
		// put in real action
		board.data = stack.board.data;
		board.reconnectAll();
		if(testAlgoBFSIterator % 10 == 0 || board.isAllClientOK())
			refreshBoard();
	}
	
	testAlgoBFSTimeout = setTimeout(testAlgoBFS, testAlgoBFSTimeoutTimer);
}
