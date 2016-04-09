$(function(){
	board = new Board();
	board.initialize();
	
	$('.board').width((boardSize * 64) + 9);
	refreshBoard();
});

function bindCellClick(){
	$('.board table tr td > span').click(function(){
		isLeft 	= $(this).hasClass('left');
		isUp 	= $(this).hasClass('up');
		isRight = $(this).hasClass('right');
		isDown 	= $(this).hasClass('down');
		
		// special pipe-1
		isPipe1	= $(this).hasClass('pipe-1');
		
		if(isPipe1){
			if(isLeft){ 	cellChangeState($(this), 'up'); }
			if(isUp){ 		cellChangeState($(this), 'left'); }
			if(isRight){ 	cellChangeState($(this), 'up'); }
			if(isDown){ 	cellChangeState($(this), 'left'); }
		}else{
			if(isLeft){ 	cellChangeState($(this), 'up'); }
			if(isUp){ 		cellChangeState($(this), 'right'); }
			if(isRight){ 	cellChangeState($(this), 'down'); }
			if(isDown){ 	cellChangeState($(this), 'left'); }
		}
	});
}

function cellChangeState(cellSpan, direction){
	row = parseInt(cellSpan.attr('row'));
	cell = parseInt(cellSpan.attr('cell'));

	//console.log('clicked (' + (row + 1) + 'x' + (cell + 1) + ') -> ' + direction + '!'); // add +1 for human readable
	cellObject = board.cellChangeState(row, cell, direction);
	
	// reDraw / refresh
	refreshBoard();
}

function refreshBoard(){
	board.reDraw($('.board table'));
	bindCellClick();
}
