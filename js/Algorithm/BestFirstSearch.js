// ================================================================================
/*
 * do it
 * */
testAlgoBFSlog = [];
testAlgoBFSstack = [];
testAlgoBFSTimeout = 0;
testAlgoBFSTimeoutTimer = 1;
function testAlgoBFS(){
	if(board.isAllClientOK()){
		$('.BFS-stop-btn').addClass('hide');
		$('.BFS-btn').removeAttr('disabled');
		return true;
	}
	
	
	
	
	parentState = board.data;
	
	moves = [];
	for(move in board.legalMoves){
		testBoard = board.clone();
		testBoard.move(board.legalMoves[move]);
		
		// check state
		if(testAlgoBFSlog.indexOf(JSON.stringify(testBoard.data)) != -1) continue;
		
		f = calculateDistance(testBoard);
		moves.push({
			'move' : board.legalMoves[move],
			'f' : f
		});
	}
	
	if(moves.length > 0){
		//sort
		moves.sort(function(a, b){
			if(a.f > b.f) return 1;
			if(a.f < b.f) return -1;
			return 0;
		});
		
		moveTile(moves[0].move);
		testAlgoBFSlog.push(JSON.stringify(board.data));
		testAlgoBFSstack.push(JSON.stringify(board.data));
	}else{
		newStack = testAlgoBFSstack.pop();
		board.data = JSON.parse(newStack);
		board.calculateSituation();
		board.refreshBoard();
		resizeStyle();
	}
	
	testAlgoBFSTimeout = setTimeout(testAlgoBFS, 3);
}
