$(function(){
	loger = new Loger();
	if(debug) boardSize = 3;
	
	init_game();
	change_theme('simple');
	
	//  SA GA BFS AStar
	$('.AI-list > p').hide();
	$('SA,BFS,AStar,AStarServer,BFSServer'.split(',')).each(function(k, v){
		$('.' + v + '-btn').parent().show();
	});
	
	
	$.get('AI/config.json', function(configJSON){
		var api_url = 'http://' + configJSON.http_host + ':' + configJSON.http_port;
		API.setUrl(api_url);
		
		// test
		API.get({
			method: 'ping', 
			params: {
				
			}
		}, function(d){
			console.log('API response: ' + d);
		});
		
	});
});

function addslashes( str ) {
    return (str + '').replace(/[\\"']/g, '\\$&').replace(/\u0000/g, '\\0');
}

var jsonstring;
function debug_init(){
	jsonstring = JSON.stringify(board.data);
}
function api_debug(method, params){
	var json = {method: method, params: params};
	
	console.log('rpc send:');
	data = {
		jsonrpc	: "2.0",
		method	: method,
		params	: params
	};
	var req = JSON.stringify(data);
	console.log(req);
	
	API.get(json, function(d){
		console.log('rpc reply:');
		console.log(d);
	}, function(d){
		console.log('error:');
		console.log(d);
	});
}

function init_game(newSize){
	boardSize = newSize === undefined ? boardSize : newSize;
	
	board = new Board();
	board.initialize();
	board.createLevel();
	board.randomize();
	loger.reset();
	loger.write('level created [' + boardSize + ' x ' + boardSize + ']!');
	
	var padding = 10;
	$('.board').width((boardSize * 40) + (padding));
	$('.board').height((boardSize * 40) + (padding));
	refreshBoard();
}

function change_theme(theme){
	$('.board').removeClass('theme-simple theme-plumbing theme-network');
	$('.board').addClass('theme-' + theme);
}

function bindCellClick(){
	$('.board table tr td > span').click(function(){
		if($(this).hasClass('blank')) return;
		
		isLeft 	= $(this).hasClass('left');
		isUp 	= $(this).hasClass('up');
		isRight = $(this).hasClass('right');
		isDown 	= $(this).hasClass('down');
		
		// special pipe-1
		isPipe1	= $(this).hasClass('pipe-1');
		direction = '';
		
		if(isPipe1){
			if(isLeft){ 	direction = 'up'; }
			if(isUp){ 		direction = 'left'; }
			if(isRight){ 	direction = 'up'; }
			if(isDown){ 	direction = 'left'; }
		}else{
			if(isLeft){ 	direction = 'up'; }
			if(isUp){ 		direction = 'right'; }
			if(isRight){ 	direction = 'down'; }
			if(isDown){ 	direction = 'left'; }
		}
		
		cellChangeState($(this), direction)		
		loger.write('[' + $(this).attr('row') + ', ' + $(this).attr('cell') + '] ' + $(this).attr('cell-type') + ' -> ' + direction);
	});
}

function cellChangeState(cellSpan, direction){
	row = parseInt(cellSpan.attr('row'));
	cell = parseInt(cellSpan.attr('cell'));
	
	//console.log('clicked (' + (row + 1) + 'x' + (cell + 1) + ') -> ' + direction + '!'); // add +1 for human readable
	cellObject = board.cellChangeState(row, cell, direction);
	
	// reDraw / refresh
	refreshBoard();
	
	if(board.isAllClientOK())
		$('.alert-win').modal();
}

function refreshBoard(){
	board.reDraw($('.board table'));
	bindCellClick();
}
