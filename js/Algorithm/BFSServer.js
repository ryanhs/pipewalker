// ================================================================================
/*
 * do it
 */
testAlgoBFSServerIterator = 0;
testAlgoBFSServerTimeout = 0;
testAlgoBFSServerTimeoutTimer = 4;
var testAlgoBFSServerStart;
function testAlgoBFSServer(){
	
	testAlgoBFSServerTimeout = false;
	testAlgoBFSServerIterator = 0;
	
	var boardId = 0;
	var currentJSONBoard = JSON.stringify(board.data);
	
	console.log('BFS Server: start!');
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
			method: 'ai_BFS',
			params: {
				'id': boardId,
			}
		}, function(d){
			console.log('BFS Server: ' + d.status + '!');
			
			function do_BFSServer(){
				API.get({
					method: 'ai_getBoard',
					params: {
						'id': boardId,
					}
				}, function(d){
					board.data = JSON.parse(d.json);
					refreshBoard();
					console.log('BFS Server: refreshBoard #' + testAlgoBFSServerIterator + '!');
					
					if(!board.isAllClientOK()){
						testAlgoBFSServerIterator++;
						testAlgoBFSServerTimeout = setTimeout(do_BFSServer, 250);
					}else{
						API.get({
							method: 'ai_closeBoard',
							params: {
								'id': boardId,
							}
						}, function(d){
							console.log('BFS Server: finish!');
							$('.BFSServer-stop-btn').addClass('hide');
							$('.BFSServer-btn').removeAttr('disabled');
						});
					}
				});
			}
			
			testAlgoBFSServerTimeout = setTimeout(do_BFSServer, 1000);
			//~ $('.BFSServer-stop-btn').addClass('hide');
			//~ $('.BFSServer-btn').removeAttr('disabled');
		});
	});	
}
