// ================================================================================
/*
 * do it
 * */
testAlgoSAlog = [];
testAlgoSAstack = [];
testAlgoSATimeout = 0;
testAlgoSATimeoutTimer = 1;
function testAlgoSA(){
	if(board.isAllClientOK()){
		$('.SA-stop-btn').addClass('hide');
		$('.SA-btn').removeAttr('disabled');
		return true;
	}
	
	// local var
	var temperature = 1000,
		iterator = 0,
		solution, 	newSolution,
		result,		newResult
		;
	
	solution = AI_random_solution();
	result = AI_evaluator(solution);
	function SA_run(){
		if(result.isAllClientOK){
			$('.SA-stop-btn').click();
			$('.alert-win').modal();
			return;
		}
		
		iterator++;
		newSolution = $.extend(true, [], solution); // jQuery hack
		newSolution = AI_modifier(newSolution);
		newResult = AI_evaluator(newSolution);
		
		if(newResult.result > result.result){
			solution = newSolution;
			result = newResult;
		}else{
			var d = newResult.result - result.result;
			var e = Math.log(-d/temperature) + 1;
			var r = mt_rand(1000) / 1000;
			
			if(e > r){
				solution = newSolution;
				result = newResult;
			}
		}
		
		/*
		if(iterator % 1000 == 0 && result.result < 2){
			solution = AI_random_solution();
			result = AI_evaluator(solution);
			temperature = 1000;
		}
		*/
		
		temperature *= 0.99;
		
		// debug
		logStr = ('SA: i#' + iterator + ' -> t#' + (temperature).toFixed(2) + 'ᵒ -> ' + result.result);
		console.log(logStr);
		loger.write(logStr);
		board.data = result.board.data;
		board.reconnectAll();
		refreshBoard();
				
		testAlgoSATimeout = setTimeout(SA_run, testAlgoSATimeoutTimer);
	}
	
	testAlgoSATimeout = setTimeout(SA_run, testAlgoSATimeoutTimer);
	
}
