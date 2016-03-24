function navigation(page)
{	
 	var currrent1="", current2="", current3="", current4="", current5="", current6="", current7= "";
	switch(page) {
		case 0: break;
		case 1: var current1='id="current"'; break;
		case 2: var current2='id="current"'; break; 
		case 3: var current3='id="current"'; break;
		case 4: var current4='id="current"'; break;
		case 5: var current5='id="current"'; break;
		case 6: var current6='id="current"'; break;
		case 7: var current7='id="current"'; break;
		default: document.write('javascript error');
	}
	document.write('<!-- navigation starts-->');	
	document.write('<div id="menu2">');
	document.write('<ul>');
	document.write('<li><a href="data/resultant.html" ', current1,'>resultant</a></li>');
	document.write('<li><a href="data/2-level.html" ', current2, '>2-level</a></li>');
	document.write('</ul>');
	document.write('<!-- navigation ends-->	');
	document.write('</div>');
}
