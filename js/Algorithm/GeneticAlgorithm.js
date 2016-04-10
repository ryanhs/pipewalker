// ================================================================================
/*
 * do it
 * */
testAlgoGAlog = [];
testAlgoGAstack = [];
testAlgoGATimeout = 0;
testAlgoGATimeoutTimer = 20;
function testAlgoGA(){
	if(board.isAllClientOK()){
		$('.GA-stop-btn').addClass('hide');
		$('.GA-btn').removeAttr('disabled');
		return true;
	}
	
	// some config
	var maxPopulation = 100,
		maxBestRanking = 10,
		population = [],
		statistics = [],
		iterator = 0;
	
	//start
	population = GA_init_population(maxPopulation);
	statistics = GA_fitness_function(population);
	
	function GA_run(){
		if(population[0].board.isAllClientOK()){
			$('.GA-stop-btn').addClass('hide');
			$('.GA-btn').removeAttr('disabled');
			return true;
		}
		iterator++;
		
		// algo
		population = population.slice(0, maxBestRanking);
		population = GA_crossover(population, maxPopulation);
		population = GA_mutation(population);
		statistics = GA_fitness_function(population);
		
		// debug
		logStr = ('[GA] i=' + iterator + ' -> m=' + (statistics.mean).toFixed(2));
		console.log(logStr);
		loger.write(logStr);
		board.data = population[0].board.data;
		board.reconnectAll();
		refreshBoard();
				
		testAlgoGATimeout = setTimeout(GA_run, testAlgoGATimeoutTimer);
	}
	
	testAlgoGATimeout = setTimeout(GA_run, testAlgoGATimeoutTimer);
}

function GA_init_population(size){
	var population = [],
		i = 0,
		tmpBoard = null;
		
	for(i = 0; i < size; i++){
		population.push({
			'solution': AI_random_solution(),
			'board': null,
			'evaluator': null,
		});
	}
	return population;
}

function GA_fitness_function(population){
	var idx = 0,
		statistics = {
			'mean': 0,
			'count': 0
		},
		tmpResult = 0;
	
	for(idx in population){
		population[idx].evaluator = AI_evaluator(population[idx].solution);
		population[idx].board = population[idx].evaluator.board;
		
		tmpResult += population[idx].evaluator.result;
		
		statistics.count++;
		statistics.mean = tmpResult / statistics.count;
	}
	
	population.sort(function(a, b){
		if(a.evaluator.result < b.evaluator.result) return  1;
		if(a.evaluator.result > b.evaluator.result) return  -1;
		return  0;
	});
		
	return statistics;
}

function GA_crossover(parentPopulation, maxPopulation){
	var i = 0,
		row = 0,
		cell = 0,
		tmpCell,
		parentA,
		parentB,
		population = [];
	
	for(i = 0; i < maxPopulation; i++){
		parentA = parentPopulation[mt_rand(parentPopulation.length - 1)];
		parentB = parentPopulation[mt_rand(parentPopulation.length - 1)];
			
		for(row = 0; row < boardSize; row++){
			for(cell = 0; cell < boardSize; cell++){
				
				if(mt_rand(1) == 1){ // %50 chance
					parentA.solution[row][cell] = parentB.solution[row][cell];
				}
				
			}
		}
		
		population.push({
			'solution': parentA.solution,
			'board': null,
			'evaluator': null,
		});
	}
	
	return population;
}

function GA_mutation(population){
	for(idx in population){
		population[idx].solution = AI_modifier(population[idx].solution);
	}
	
	return population;
}
