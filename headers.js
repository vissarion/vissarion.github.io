function navigation(page)
{	
 	var currrent1="", current2="", current3="", current4="", current5="", current6="", current7= "";
	switch(page) {
		case 1: var current1='id="current"'; break;
		case 2: var current2='id="current"'; break; 
		case 3: var current3='id="current"'; break;
		case 4: var current4='id="current"'; break;
		case 5: var current5='id="current"'; break;
		case 6: var current6='id="current"'; break;
		case 7: var current7='id="current"'; break;
		default: document.write('javascript error');
	}
	document.write('<div id="header">');
	document.write('		<table>');
	document.write('			<tr>');
	switch(page) {		
        case 1: 
			document.write('			<td class="img"><img src="images/1.jpg" width="80" height="80" alt="photo" class="float-left" /></td>');
			break;
		case 2:
			document.write('			<td class="img"><img src="images/5.jpg" width="80" height="80" alt="photo" class="float-left" /></td>');
			break;
		case 3:
			document.write('			<td class="img"><img src="images/3.jpg" width="80" height="80" alt="photo" class="float-left" /></td>');
			break;
		case 4:
			document.write('			<td class="img"><img src="images/4.jpg" width="80" height="80" alt="photo" class="float-left" /></td>');
			break;
		case 5:
			document.write('			<td class="img"><img src="images/2.jpg" width="80" height="80" alt="photo" class="float-left" /></td>');
			break;
               default:
			document.write('			<td class="img"><img src="images/3.jpg" width="80" height="80" alt="photo" class="float-left" /></td>');
			break;
	}
	document.write('			<td class="info">');
	document.write('			<h2>Vissarion Fisikopoulos &nbsp::&nbsp Computer Engineer &amp; Research Scientist</h2><hr>');            
	//document.write('            <a href="http://dblp.uni-trier.de/pers/hd/f/Fisikopoulos:Vissarion.html"><img src="images/dblp.png" width="60" height="60" alt="photo" class="float-left" /></a>');
    //document.write('            <a href="https://scholar.google.gr/citations?user=Tro-X7MAAAAJ"><img src="images/scholar.png" width="60" height="60" alt="photo" class="float-left" /></a>');
    //document.write('            <a href="https://github.com/vissarion"><img src="images/GitHub.png" width="60" height="60" alt="photo" class="float-left" /></a>');
    //document.write('            <a href="https://www.linkedin.com/public-profile/settings?trk=prof-edit-edit-public_profile"><img src="images/linkedin-logo.png" width="60" height="60" alt="photo" class="float-left" /></a>');
    document.write('			<i>email</i>: vissarion [dot] fisikopoulos [at] gmail [dot] com<br/>');
   	document.write('			<i>location</i>:  Athens, Greece<br/>');
    document.write('			</td>');
	document.write('			</tr>');
	document.write('		</table>');
	document.write('	</div>'); 
	document.write('<!-- navigation starts-->');	
	document.write('<div id="menu">');
	document.write('<ul>');
	document.write('<li><a href="index.html" ', current1,'>Home</a></li>');
	document.write('<li><a href="publications.html" ', current2, '>Publications</a></li>');
	document.write('<li><a href="software.html" ', current3, '>Software</a></li>');
    document.write('<li><a href="data.html" ', current4, '>Data</a></li>');
	//document.write('<li><a href="other.html" ', current5, '>Other</a></li>');
	document.write('</ul>');
	document.write('<!-- navigation ends-->	');
	document.write('</div>');
}
