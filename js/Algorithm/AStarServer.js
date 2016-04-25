// ================================================================================
/*
 * do it
 */
testAlgoAStarServerIterator = 0;
testAlgoAStarServerTimeout = 0;
testAlgoAStarServerTimeoutTimer = 4;
var testAlgoAStarServerStart;
function testAlgoAStarServer(){
	
	testAlgoAStarServerTimeout = false;
	testAlgoAStarServerIterator = 0;
	
	var boardId = 0;
	var currentJSONBoard = JSON.stringify(board.data);
	
	console.log('A* Server: start!');
	API.get({
		method: 'ai_newBoard',
		params: {
			'size': boardSize,
			'json': currentJSONBoard
		}
	}, function(d){
		boardId = d.id;
		console.log('boardId : ' + boardId);
		
		
		API.get({
			method: 'ai_A*',
			params: {
				'id': boardId,
			}
		}, function(d){
			console.log('A* Server: ' + d.status + '!');
			
			function do_AStarServer(){
				API.get({
					method: 'ai_getBoard',
					params: {
						'id': boardId,
					}
				}, function(d){
					board.data = JSON.parse(d.json);
					refreshBoard();
					console.log('A* Server: refreshBoard #' + testAlgoAStarServerIterator + '!');
					
					if(!board.isAllClientOK()){
						testAlgoAStarServerIterator++;
						testAlgoAStarServerTimeout = setTimeout(do_AStarServer, 250);
					}else{
						API.get({
							method: 'ai_closeBoard',
							params: {
								'id': boardId,
							}
						}, function(d){
							console.log('A* Server: finish!');
							$('.AStarServer-stop-btn').addClass('hide');
							$('.AStarServer-btn').removeAttr('disabled');
						});
					}
				});
			}
			
			testAlgoAStarServerTimeout = setTimeout(do_AStarServer, 1000);
			//~ $('.AStarServer-stop-btn').addClass('hide');
			//~ $('.AStarServer-btn').removeAttr('disabled');
		});
	});	
}
