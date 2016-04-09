
function Board() {
	this.data = [];
	this.defaultDataType = [
		['blank', 'client', 'pipe-2', 'pipe-1'],
		['pipe-3', 'pipe-2', 'pipe-1', 'pipe-2'],
		['pipe1', 'source', 'pipe-2', 'pipe-1'],
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
	
	// return cell
	this.cellChangeState = function(row, cell, direction){
		this.data[row][cell].direction = direction;
		this.reconnectAll();
		return this.data[row][cell];
	}
	
	this.reconnectAll = function(){
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
				branchCoordinate = [-1, -1];
				
				if(branches[branch] == 'up'){
					branchCoordinate = [
						(currentCoordinate[0] - 1) == -1 ? boardSize - 1 : (currentCoordinate[0] - 1), // check to reverse
						currentCoordinate[1],
					];
				}
				if(branches[branch] == 'right'){
					branchCoordinate = [
						currentCoordinate[0],
						(currentCoordinate[1] + 1) == boardSize ? 0 : (currentCoordinate[1] + 1), // check to reverse
					];
				}
				if(branches[branch] == 'down'){
					branchCoordinate = [
						(currentCoordinate[0] + 1) == boardSize ? 0 : (currentCoordinate[0] + 1), // check to reverse
						currentCoordinate[1],
					];
				}
				if(branches[branch] == 'left'){
					branchCoordinate = [
						currentCoordinate[0],
						(currentCoordinate[1] - 1) == -1 ? boardSize - 1 : (currentCoordinate[1] - 1), // check to reverse
					];
				}
				
				if(branchCoordinate[0] == -1 && branchCoordinate[1] == -1) continue; // maybe imposible but, oke..
				
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
