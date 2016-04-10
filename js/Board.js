Math.randomRange = function(min, max) {
    if (max) {
        return (min + Math.floor(Math.random() * (max - min + 1)));
    } else if (min) {
        return (Math.floor(Math.random() * min + 1))
    } else {
        return (Math.floor(Math.random() * 101));
    }
}

function mt_rand(max){
	//return Math.floor((Math.random() * 1000) % (max + 1));
	
	return Math.randomRange(0, max);
}

function rand_direction(){
	r = mt_rand(4);
	return r == 1 ? 'left'
		: (r == 2 ? 'up'
		: (r == 2 ? 'right'
		: 'down'));
}

function Board() {
	this.data = [];
	this.goal = [];
	this.defaultDataType = [
		['blank', 'client', 'pipe-2', 'pipe-1'],
		['pipe-3', 'pipe-2', 'pipe-1', 'pipe-2'],
		['pipe1', 'source', 'pipe-2', 'pipe-2'],
		['pipe-1', 'blank', 'pipe-1', 'blank'],
	];
	this.defaultDataActive = [
		[false, true, true, false],
		[false, false, true, false],
		[false, true, true, false],
		[false, false, false, false],
	];
	this.defaultDataDirection = [
		['left', 'right', 'down', 'left'],
		['left', 'right', 'up', 'left'],
		['left', 'right', 'left', 'left'],
		['left', 'left', 'left', 'left'],
	];
	
	this.initialize = function(){
		this.data = [];
		
		for(row = 0; row < boardSize; row++){
			tmp = [];
			for(cell = 0; cell < boardSize; cell++){
				
				if(debug){
					tmp.push({
						type: 	this.defaultDataType[row][cell],
						active: this.defaultDataActive[row][cell],
						direction: this.defaultDataDirection[row][cell],
					})
				}else{
					tmp.push({
						type: 	'blank',
						active: false,
						direction: 'up',
					})
				}
			}
			this.data.push(tmp);	
		}
		
		return this;
	}
	
	this.createLevel = function(){		
		// source placement
		sourceCoordinate = [mt_rand(boardSize-1), mt_rand(boardSize-1)];
		sourceCell = this.data[sourceCoordinate[0]][sourceCoordinate[1]];
		sourceCell.type = 'source';
		sourceCell.direction = rand_direction();
		
		// have fun!
		maxCell = boardSize * boardSize;
		filledBranch = [JSON.stringify(sourceCoordinate)];
		freeBranch = [JSON.stringify(this.getNeighborCoordinate(sourceCoordinate, sourceCell.direction))];
		while(filledBranch.length < maxCell && freeBranch.length > 0){
			currentCoordinate = JSON.parse(freeBranch.splice(0, 1)[0]);
			currentCell = this.data[currentCoordinate[0]][currentCoordinate[1]];
			
			// makesure added to fill
			filledBranch.push(JSON.stringify(currentCoordinate));
			//console.log(currentCoordinate);
			
			if(freeBranch.length == 0){ // only use pipe
				currentCell.type = 'pipe-1|pipe-2|pipe-3|pipe-3|pipe-3'.split('|')[mt_rand(4)];
			}
			else{
				currentCell.type = 'pipe-1|pipe-2|pipe-3|client|client'.split('|')[mt_rand(4)];
			}
			
			direction = 'up';
			
			if(currentCell.type == 'client'){
				directions = ['up', 'right', 'down', 'left'];
				for(idx in directions){
					direction = directions[idx];
					
					okBranch = null;
					branches = this.cellActiveBranches(currentCell.type, direction);
					for(branch in branches){
						neighborCoordinate = this.getNeighborCoordinate(currentCoordinate, branches[branch]);					
						neighborCell = this.data[neighborCoordinate[0]][neighborCoordinate[1]];
						
						neighborConnection = this.cellActiveBranches(neighborCell.type, neighborCell.direction);
						connectionReverse = branches[branch] == 'up' ? 'down'
									 : (branches[branch] == 'right' ? 'left'
									 : (branches[branch] == 'down' ? 'up'
									 : ('right')));
									 
						
						if(neighborConnection.indexOf(connectionReverse) != -1) okBranch = branches[branch];
					}
					if(okBranch != null) break;
				}
			}else{
				okBranch = null;
				while(true){
					direction = rand_direction();
					branches = this.cellActiveBranches(currentCell.type, direction);
					
					okBranch = null;
					for(branch in branches){
						neighborCoordinate = this.getNeighborCoordinate(currentCoordinate, branches[branch]);					
						neighborCell = this.data[neighborCoordinate[0]][neighborCoordinate[1]];
						
						neighborConnection = this.cellActiveBranches(neighborCell.type, neighborCell.direction);
						connectionReverse = branches[branch] == 'up' ? 'down'
									 : (branches[branch] == 'right' ? 'left'
									 : (branches[branch] == 'down' ? 'up'
									 : ('right')));
									 
						
						if(neighborConnection.indexOf(connectionReverse) != -1) okBranch = branches[branch];
					}
					if(okBranch != null) break;
				}
				//console.log(okBranch);
			}
			
			currentCell.direction = direction;
			if(currentCell.type == 'client') continue;
			
			branches = this.cellActiveBranches(currentCell.type, currentCell.direction);
			//console.log(branches);
			for(branch in branches){
				neighborCoordinate = this.getNeighborCoordinate(currentCoordinate, branches[branch]);
				neighborStringify = JSON.stringify(neighborCoordinate);
				if(!(
					okBranch[0] == neighborCoordinate[0]
				&&  okBranch[1] == neighborCoordinate[1]
				)){
					if(freeBranch.indexOf(neighborStringify) == -1
					&& filledBranch.indexOf(neighborStringify) == -1)
						freeBranch.push(neighborStringify);
				}
			}
		}
		
		// check
		this.reconnectAll();
		
		//polish, make it harder
		for(row = 0; row < boardSize; row++){
			for(cell = 0; cell < boardSize; cell++){
				if('pipe-1|pipe-2|pipe-3'.split('|').indexOf(this.data[row][cell].type) != -1){
					tmpCoordinate = [row, cell];
					tmpCell = this.data[row][cell];
					
					realType = tmpCell.type;
					realDirection = tmpCell.direction;
					
					tmpCell.type = 'blank';
					tmpCell.direction = 'up';
					
					this.reconnectAll();
					
					// undo if its wrong :p
					if(!this.isAllClientOK()){
						tmpCell.type = realType;
						tmpCell.direction = realDirection;
						this.reconnectAll();
					}else{
						//console.log('removed [' + row + ',' + cell + '] ' + realType);
					}
				}
			}
		}
		
		// path for pipe-1
		for(row = 0; row < boardSize; row++){
			for(cell = 0; cell < boardSize; cell++){
				tmpCell = this.data[row][cell];
				if(tmpCell.type == 'pipe-1' && tmpCell.direction == 'down')
					tmpCell.direction = 'up';
				if(tmpCell.type == 'pipe-1' && tmpCell.direction == 'right')
					tmpCell.direction = 'left';
			}
		}
		
		this.reconnectAll();
		
		// set goal;
		/*
		 * copy this.data to this.goal
		 * */
		this.goal = $.extend(true, [], this.data); // jQuery hack
	}
	
	this.randomize = function(){
		var row, cell, tmpCell; // local variable scope
		for(row = 0; row < boardSize; row++){
			for(cell = 0; cell < boardSize; cell++){
				tmpCell = this.data[row][cell];
				tmpCell.direction = rand_direction(); // random direction
			}
		}
		
		// path for pipe-1
		for(row = 0; row < boardSize; row++){
			for(cell = 0; cell < boardSize; cell++){
				tmpCell = this.data[row][cell];
				if(tmpCell.type == 'pipe-1' && tmpCell.direction == 'down')
					tmpCell.direction = 'up';
				if(tmpCell.type == 'pipe-1' && tmpCell.direction == 'right')
					tmpCell.direction = 'left';
			}
		}
		this.reconnectAll();
	}
	
	this.evaluator = function(){
		// local variable scope
		var row, 
			cell, 
			tmpCell, 
			clientConnected = 0,
			pipeConnected = 0,
			isAllClientOK = true;
		
		for(row = 0; row < boardSize; row++){
			for(cell = 0; cell < boardSize; cell++){
				tmpCell = this.data[row][cell];
				
				if(tmpCell.type == 'client' && tmpCell.active == false){
					isAllClientOK = false;
				}
				
				if(tmpCell.type == 'client' && tmpCell.active == true){
					clientConnected++;
				}
				
				if('pipe-1|pipe-2|pipe-3|'.split('|').indexOf(tmpCell.type) != -1 && tmpCell.active == true){
					pipeConnected++;
				}
				
			}
		}
		
		return {
			clientConnected : clientConnected,
			pipeConnected : pipeConnected,
			result : (clientConnected * 3) + (pipeConnected * 1),
			
			isAllClientOK : isAllClientOK,
			board: this, // usefull of AI
		}
	}

	this.isAllClientOK = function(){
		return (this.evaluator()).isAllClientOK;
	}
	
	this.getNeighborCoordinate = function(currentCoordinate, direction){
		if(direction == 'up'){ 		return this.getUpCoordinate(currentCoordinate); }
		if(direction == 'right'){ 	return this.getRightCoordinate(currentCoordinate); }
		if(direction == 'down'){ 	return this.getDownCoordinate(currentCoordinate); }
		if(direction == 'left'){ 	return this.getLeftCoordinate(currentCoordinate); }
	}
	
	this.getLeftCoordinate = function(currentCoordinate){
		return [
			currentCoordinate[0],
			(currentCoordinate[1] - 1) == -1 ? boardSize - 1 : (currentCoordinate[1] - 1), // check to reverse
		]
	}
	
	this.getUpCoordinate = function(currentCoordinate){
		return [
			(currentCoordinate[0] - 1) == -1 ? boardSize - 1 : (currentCoordinate[0] - 1), // check to reverse
			currentCoordinate[1],
		]
	}
	
	this.getRightCoordinate = function(currentCoordinate){
		return [
			currentCoordinate[0],
			(currentCoordinate[1] + 1) == boardSize ? 0 : (currentCoordinate[1] + 1), // check to reverse
		]
	}
	this.getDownCoordinate = function(currentCoordinate){
		return [
			(currentCoordinate[0] + 1) == boardSize ? 0 : (currentCoordinate[0] + 1), // check to reverse
			currentCoordinate[1],
		]
	}
	
	// return cell
	this.cellChangeState = function(row, cell, direction){
		this.data[row][cell].direction = direction;
		this.reconnectAll();
		return this.data[row][cell];
	}
	
	this.reconnectAll = function(){
		var row, cell; // local variable scope
		sourceCoordinate = [-1, -1];
		
		for(row = 0; row < boardSize; row++){
			for(cell = 0; cell < boardSize; cell++){
				if(this.data[row][cell].type == 'source'){
					sourceCoordinate = [row, cell];
				}
				
				this.data[row][cell].active = false;
			}
		}
		
		if(sourceCoordinate[0] == -1 && sourceCoordinate[1] == -1 ) return; // there is no source!!!
		
		openSet = [JSON.stringify(sourceCoordinate)];
		
		parent = this;
		function do_reconnect(){
			if(openSet.length == 0) return; // finish
			//console.log(openSet);
			
			currentCoordinate = JSON.parse(openSet.splice(0, 1)[0]);
			currentCell = parent.data[currentCoordinate[0]][currentCoordinate[1]];
			branches = parent.cellActiveBranches(currentCell.type, currentCell.direction);
			
			for(branch in branches){
				branchCoordinate = parent.getNeighborCoordinate(currentCoordinate, branches[branch])
				branchCell = parent.data[branchCoordinate[0]][branchCoordinate[1]];
				if(branchCell.active) continue; // already active	
				
				branchConnection = parent.cellActiveBranches(branchCell.type, branchCell.direction);
				connectionReverse = branches[branch] == 'up' ? 'down'
								 : (branches[branch] == 'right' ? 'left'
								 : (branches[branch] == 'down' ? 'up'
								 : ('right')))
				
				if(
					(branchConnection.indexOf(connectionReverse) != -1)
				&&  (openSet.indexOf(JSON.stringify(branchCoordinate)) == -1)
				){
					openSet.push(JSON.stringify(branchCoordinate));
					branchCell.active = true;
				}
			}
			
			do_reconnect();
		}
		
		do_reconnect();
	}
	
	this.cellActiveBranches = function(type, direction){
		if(type == 'source'){
			if(direction == 'left') 	return ['left'];
			if(direction == 'up') 		return [	    'up'];
			if(direction == 'right') 	return [			  'right'];
			if(direction == 'down') 	return [					   'down'];
		}
		if(type == 'pipe-1'){
			if(direction == 'left') 	return ['left'		, 'right'];
			if(direction == 'up') 		return [		'up'		 , 'down'];
			if(direction == 'right') 	return ['left'		, 'right'];
			if(direction == 'down') 	return [		'up'		 , 'down'];
		}
		if(type == 'pipe-2'){
			if(direction == 'left') 	return ['left', 'up'];
			if(direction == 'up') 		return [		'up', 'right'];
			if(direction == 'right') 	return [			  'right', 'down'];
			if(direction == 'down') 	return ['left'				 , 'down'];
		}
		if(type == 'pipe-3'){
			if(direction == 'left') 	return ['left', 'up', 'right'];
			if(direction == 'up') 		return [		'up', 'right', 'down'];
			if(direction == 'right') 	return ['left'		, 'right', 'down'];
			if(direction == 'down') 	return ['left', 'up'		 , 'down'];
		}
		
		if(type == 'client'){
			if(direction == 'left') 	return ['left'];
			if(direction == 'up') 		return [	    'up'];
			if(direction == 'right') 	return [			  'right'];
			if(direction == 'down') 	return [					   'down'];
		}
		
		return [];
	}
	
	this.reDraw = function(tableDOM){
		tableDOM.empty();
		//console.log('reDraw board!');
			
		for(row = 0; row < boardSize; row++){
			rowDOM = $('<tr>');
			for(cell = 0; cell < boardSize; cell++){
				cellDOM = $('<td>');
				cellSpan = $('<span>');
				cellObject = this.data[row][cell];
				
				if('source|client'.split('|').indexOf(cellObject.type) != -1){
					cellSpan.append($('<span>').addClass('half'));
				}
				
				cellSpan.attr('cell-type', cellObject.type);
				cellSpan.attr('row', row);
				cellSpan.attr('cell', cell);
				cellSpan.addClass(cellObject.type);
				cellSpan.addClass(cellObject.direction);
				if(cellObject.active || cellObject.type == 'source') cellSpan.addClass('active');
				
				cellDOM.append(cellSpan);
				rowDOM.append(cellDOM);
			}
			tableDOM.append(rowDOM);
		}
		
		return this;
	}
}
