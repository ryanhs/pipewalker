function AI_randomSolution(){
	// local variable scope
	var solution = [],
		row = 0,
		rowArr = [],
		cell = 0,
		tmpCell;
	
	for(row = 0; row < boardSize; row++){
		rowArr = [];
		for(cell = 0; cell < boardSize; cell++){
			tmpCell = board.data[row][cell];
			
			if('pipe-1|pipe-2|pipe-3|client|source'.split('|').indexOf(tmpCell.type) != -1){
				rowArr.push(rand_direction());
			}else{
				rowArr.push(false);
			}
		}
		solution.push(rowArr);
	}
	
	return solution;
}

function AI_getCurrentSolution(){
	var solution = [],
		row = 0,
		rowArr = [],
		cell = 0,
		tmpCell;
	
	for(row = 0; row < boardSize; row++){
		rowArr = [];
		for(cell = 0; cell < boardSize; cell++){
			tmpCell = board.data[row][cell];
			
			if('pipe-1|pipe-2|pipe-3|client|source'.split('|').indexOf(tmpCell.type) != -1){
				rowArr.push(tmpCell.direction);
			}else{
				rowArr.push(false);
			}
		}
		solution.push(rowArr);
	}
	
	return solution;
}

function AI_modifier(solution){
	// local variable scope
	var row = 0,
		cell = 0,
		tmpCell;
		
	while(true){
		row = mt_rand(boardSize - 1);
		cell = mt_rand(boardSize - 1);
		
		if(!(solution[row][cell] == false)){
			solution[row][cell] = rand_direction();
			break;
		}
	}
	
	return solution;
}

function AI_evaluator(solution){
	// local variable scope
	var row = 0,
		cell = 0,
		tmpCell,
		tmpBoard,
		evaluator;
	
	tmpBoard = new Board();
	tmpBoard.data = $.extend(true, [], board.data); // jQuery hack
	
	for(row = 0; row < boardSize; row++){
		for(cell = 0; cell < boardSize; cell++){
			
			if(solution[row][cell] != false){
				tmpBoard.data[row][cell].direction = solution[row][cell];
			}else{
				tmpBoard.data[row][cell].direction = false;
			}
			
		}
	}
	
	tmpBoard.reconnectAll();
	evaluator = tmpBoard.evaluator();
	
	return evaluator;
}


