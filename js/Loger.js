
function Loger(){
	this.i = 1;
	this.box = $('.log');
	
	this.reset = function(){
		this.i = 1;
		this.box.empty();
	}
	
	this.write = function(str){
		this.box.append($('<p>').html(this.i + '. ' + str));
		this.box.scrollTop(this.box[0].scrollHeight);
		this.i++;
	}
}
