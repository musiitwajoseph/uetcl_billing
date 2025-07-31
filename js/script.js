function __(x){
	return document.getElementsByClassName(x);
}
function _(x){
	return document.getElementById(x);
}




	$(document).off('click','#loginBtn').on('click', '#loginBtn', function(e){
		login();
	});

	function login(){
		var urlPath = $("#urlPath").val();
		var username = $('#username').val();
		var password = $('#password').val();
		//var captcha = $('#captcha').val();
		//alert(username);
		if(!username || !password ){
			//alert("Please enter Username and Password1");
		}else{
			$('#loginStatus').html('<img src="'+urlPath+'images/loading.gif" alt="loading..."/> Please wait...');
			$('#loginBtn').html("Checking Credentials");
			$('#loginBtn').attr("disabled", true);
			var form_data = new FormData();
			form_data.append('username', username);
			form_data.append('password', password);
			//form_data.append('captcha', captcha);
			$.ajax({
			url: urlPath+"ajax/login.php",
			type: "POST",
			data: form_data,
			contentType: false,
			cache: false,
			processData:false,
			success: function(data){
				//alert(data);
				var	values = JSON.parse(data);
				if (values.message=='Error') {
					$('#loginBtn').html("Sign in");
					$('#loginBtn').attr('disabled',false);
					$('#loginStatus').html('<div class="text-danger">Wrong Username or Password</div>');
				}else if(values.message=='Success'){
					$('#loginStatus').html('Logged in. Redirecting, Please wait.');
					location.reload();
				}
			}
		});	
		}
	}

$(function(){
	var urlPath = $('#urlPath').val();
        $(document).off('click','#sendSMS').on('click','#sendSMS', function(e){

        	var amount = $('#amount').val();
        	var telephone = $('#telephone').val();
        	var description = $('#description').val();
        	var name = $('#name').val();
        	var receiptNo = $('#receiptNo').val();

        	if(!confirm("Do you really want to send SMS")){
        		return false;
        	}        	

        	$('#sendSMSstatus').html("Sending Please wait ...");

        	if(1){

		        var form_data = new FormData();

		        form_data.append('amount', amount);
		        form_data.append('telephone', telephone);
		        form_data.append('description', description);
		        form_data.append('name', name);
		        form_data.append('receiptNo', receiptNo);

		        $.ajax({
		            url: urlPath+"ajax/sendSMS.php",
		            type: "POST",
		            data: form_data,
		            contentType: false,
		            cache: false,
		            processData:false,
		            success: function(data){
		            	//alert(data);
        				$('#sendSMSstatus').html("SMS Sent.");
		            	//location.reload();
		            }
		        });
        	}

        });

        $(document).off('click','#cancelReceipt').on('click','#cancelReceipt', function(e){
        	var id = $(this).attr('data-id');
        	var comment = $('#comment').val();
        	if(!confirm("Do you really want to cancel this receipt")){
        		return false;
        	}

        	if(!comment){
        		alert("Enter Reason");
        		return false;
        	}
        	

        	$('#cancelReceiptStatus').html("Cancelling Please wait ...");

        	if(1){

		        var form_data = new FormData();
		        form_data.append('id', id);
		        form_data.append('comment', comment);

		        $.ajax({
		            url: urlPath+"ajax/cancelReceipt.php",
		            type: "POST",
		            data: form_data,
		            contentType: false,
		            cache: false,
		            processData:false,
		            success: function(data){
		            	//alert(data);
        				$('#cancelReceiptStatus').html("Receipt Cancelled. Reloading...");
		            	location.reload();
		            }
		        });
        	}

        });

        $(document).off('click','.delete-net-advance').on('click', '.delete-net-advance', function(e){
        	var urlPath = $('#urlPath').val();
        	var id = $(this).attr('data-id');
        	var comment = $(this).attr('data-comment-id');
        	$(this).attr('disabled',true);
        	$('.delete-net-advance').html("Deleting..."); 

        	if(1){

		        var form_data = new FormData();
		        form_data.append('id', id);
		        form_data.append('comment', comment);

		        $.ajax({
		            url: urlPath+"ajax/deleteNetAdvance.php",
		            type: "POST",
		            data: form_data,
		            contentType: false,
		            cache: false,
		            processData:false,
		            success: function(data){
		            	//alert(data);
        				$('.delete-net-advance').html("Reloading...");
		            	location.reload();
		            }
		        });
        	}
        });

        $(document).off('click','#delete_readings').on('click', '#delete_readings', function(e){
        	var urlPath = $('#urlPath').val();
        	var cust_id = $(this).attr('cust-id');
        	var month_id = $(this).attr('month-id');
        	var year_id = $(this).attr('year-id');
        	var reading_id = $(this).attr('rate-reading-id');
        	if(!confirm('Do you really want to delete readings?')){
			  return false;
		    }
        	$(this).attr('disabled',true);
        	//$('#delete_readings').html("Deleting..."); 
        	//alert(year_id);

        	if(1){

		        var form_data = new FormData();
		        form_data.append('cust_id', cust_id);
		        form_data.append('month_id', month_id);
		        form_data.append('year_id', month_id);
		         form_data.append('reading_id', reading_id);

		        $.ajax({
		            url: urlPath+"ajax/deleteReadings.php",
		            type: "POST",
		            data: form_data,
		            contentType: false,
		            cache: false,
		            processData:false,
		            success: function(data){
		            	//alert(data);
        				//$('#delete_readings').html("Reloading...");
		            	location.reload();
		            }
		        });
        	}
        });

        $(document).off('click','.netAdvanceBtn').on('click', '.netAdvanceBtn', function(e){

        	var urlPath = $('#urlPath').val();
        	var id = $(this).attr('data-id');
        	$(this).attr('disabled',true);
        	$('#netAdvanceBtnStatus'+id).html("Saving. Please wait");
        	var is = $('#is'+id).val();
        	var io = $('#io'+id).val();
        	var ip = $('#ip'+id).val();
        	var es = $('#es'+id).val();
        	var eo = $('#eo'+id).val();
        	var ep = $('#ep'+id).val();
        	var comment = $('#comment'+id).val();
        	var year = $('#year'+id).val();
        	var month = $('#month'+id).val();
        	var customer_id = $('#customer_id'+id).val();
        	var rea_id = $('#rea_id'+id).val();
        	var user_id = $('#user_id').val();

        	if(!comment){
        		alert("Enter Comment");
        		$('#netAdvanceBtnStatus'+id).html("");
        		$(this).attr('disabled',false);
        	}else{

		        var form_data = new FormData();
		        form_data.append('comment', comment);
		        form_data.append('is', is);
		        form_data.append('io', io);
		        form_data.append('ip', ip);
		        form_data.append('es', es);
		        form_data.append('eo', eo);
		        form_data.append('ep', ep);
		        form_data.append('mp_id', id);
		        form_data.append('rea_id', rea_id);
		        form_data.append('year', year);
		        form_data.append('month', month);
		        form_data.append('customer_id', customer_id);
		        form_data.append('user_id', user_id);
		        $.ajax({
		            url: urlPath+"ajax/saveNetAdvance.php",
		            type: "POST",
		            data: form_data,
		            contentType: false,
		            cache: false,
		            processData:false,
		            success: function(data){
		            	//alert(data);
        				$('#netAdvanceBtnStatus'+id).html("Saved. Reloading Page");
		            	location.reload();
		            }
		        });
        	}
        });
         $(document).off('click','.editNetAdvanceBtn').on('click', '.editNetAdvanceBtn', function(e){

        	var urlPath = $('#urlPath').val();
        	var id = $(this).attr('data-id');
        	$(this).attr('disabled',true);
        	$('#netAdvanceBtnStatus'+id).html("Saving. Please wait");
        	var is = $('#is'+id).val();
        	var io = $('#io'+id).val();
        	var ip = $('#ip'+id).val();
        	var es = $('#es'+id).val();
        	var eo = $('#eo'+id).val();
        	var ep = $('#ep'+id).val();

        	var rate__id = $('#rate__id').val();
        	var comment__id = $('#comment__id').val();
        	//alert(rate__id+'=='+comment__id);

        	var comment = $('#comment'+id).val();
        	var year = $('#year'+id).val();
        	var month = $('#month'+id).val();
        	var customer_id = $('#customer_id'+id).val();
        	var rea_id = $('#rea_id'+id).val();
        	var user_id = $('#user_id').val();

        	if(!comment){
        		alert("Enter Comment");
        		$('#netAdvanceBtnStatus'+id).html("");
        		$(this).attr('disabled',false);
        	}else{

		        var form_data = new FormData();
		        form_data.append('comment', comment);
		        form_data.append('is', is);
		        form_data.append('io', io);
		        form_data.append('ip', ip);
		        form_data.append('es', es);
		        form_data.append('eo', eo);
		        form_data.append('ep', ep);
		        form_data.append('mp_id', id);
		        form_data.append('rea_id', rea_id);
		        form_data.append('year', year);
		        form_data.append('month', month);
		        form_data.append('customer_id', customer_id);
		        form_data.append('user_id', user_id);

		         form_data.append('rate__id', rate__id);
		         form_data.append('comment__id', comment__id);
		        $.ajax({
		            url: urlPath+"ajax/editNetAdvance.php",
		            type: "POST",
		            data: form_data,
		            contentType: false,
		            cache: false,
		            processData:false,
		            success: function(data){
		            	//alert(data);
        				$('#netAdvanceBtnStatus'+id).html("Saved. Reloading Page");
		            	location.reload();
		            }
		        });
        	}
        });

        $(document).off('click','.eagle-load').on('click', '.eagle-load', function(e){
        var urlPath = $('#urlPath').val();
        e.preventDefault();
        var href = $(this).attr("href");
       	//alert(href);
        //console.log(href);    
        var hrefs = href.split('/');
        var confirmVar = (typeof hrefs[7] !== 'undefined')?hrefs[7]:"";

        if(confirmVar.toUpperCase() == "CONFIRM"){
            confirmVar = (typeof hrefs[8] !== 'undefined')?hrefs[8]:" Delete ";
            if(!confirm("Do you really want to perform this Action: "+confirmVar)){
                return 0;
            }
        }

        $('#EagleContainer').fadeTo('normal', 0.4).append('<img class="eaglepreview" src="'+urlPath+'images/loading3.gif" alt="loading..."/> <br/><center>Loading Please Wait</center>');
        
        var form_data = new FormData();
        form_data.append('href', href);
        $.ajax({
            url: urlPath+"ajax/__EAGLE_route.php",
            type: "POST",
            data: form_data,
            contentType: false,
            cache: false,
            processData:false,
            success: function(data){
                $('#EagleContainer').fadeTo('normal', 1);
                $('#EagleContainer').html(data);
                hrefs = href.split('/');
                var state = hrefs[5].replaceAll('-',' ').toUpperCase();
                document.title = 'UETCL :: '+state;
                //$('.copyright').html(href);
                window.history.replaceState({urlpath: href}, '', href);
                // window.history.replaceState({urlpath: href}, '', href);
                // window.history.replaceState({urlpath: href}, '', href);
                //window.history.go(-2);
                //location.replace(href);
            }
        });
    });
});

function toggleTabs(ID){
	var all_tabs = __("tab");
	var section = _(ID);
	var tab = _("li_"+ID);
								
	for(var i=0; i<all_tabs.length; i++){
		all_tabs[i].style.backgroundColor = "#000";
		all_tabs[i].style.color = "#FFFFFF";
		all_tabs[i].style.borderBottom = "none";
	}
				
	_("tab_section").innerHTML = section.innerHTML;	//loading content of the clicked to show
	
	tab.style.backgroundColor = "#FFF";
	tab.style.color = "#000000";
	tab.style.transition = "all linear 0.3s";
	tab.style.borderBottom = "none";	

	 $('.select-gl').select2('destroy').select2();		
}

 function metering_points(urlPath){
	var customer = _("customer");
	var customer = customer.options[customer.selectedIndex].value;
	var request;
	if (window.XMLHttpRequest){//for Chrome, Firefox, IE7+, Opera, Safari
		request = new XMLHttpRequest();
	}
	else{//for IE5, IE6
		request = new ActiveXObject("Microsoft.XMLHTTP");
	}	
	request.open("POST", urlPath+"ajax/metering_points.php", true);
	request.setRequestHeader("Content-type", "application/x-www-form-urlencoded");	

	request.onreadystatechange = function() {
		if(request.readyState == 4 && request.status == 200) {
			//4 = The connection is complete, the data was sent or retrieved.
			//200 = The file has been retrieved and you are free to do something with it
			var returned = request.responseText.split("====");			
			
			_("meterPointNumber").innerHTML = "("+returned[0]+")";
			_("meterPoint").innerHTML = returned[1];
		}
	}

	request.send("customer_id="+customer);
 }

function multipleChecker(){
	var allboxes = document.getElementsByClassName('AllCheckBoxes');
	var i=0;
	var checker = document.getElementById("checker");

	for(i=0; i<allboxes.length; i++){
		if(checker.innerHTML == "Check All"){
			allboxes[i].checked = true; 
		}else{			
			allboxes[i].checked = false; 
		}
	}

	if(checker.innerHTML == "Check All"){
		checker.innerHTML = "Uncheck All";
	}else{
		checker.innerHTML = "Check All";
	}
}



function undo(){
	var valuesInPutted = document.getElementById("valuesInPutted").value;

	valuesInPutted = valuesInPutted.split("][");
	valuesInPutted[0] = valuesInPutted[0].replace("[", "");

	valuesInPutted[valuesInPutted.length-1] = valuesInPutted[valuesInPutted.length-1].replace("]", ""); 
	
	var val = valuesInPutted[totInputted-1].split("**");
	document.getElementById("id"+val[1]).value = val[0];
	totInputted--;

}

function s(value, id, type){
	var check = value+"**"+id+"**"+type;
	var x = document.getElementById("valuesInPutted");
	var valuesInPutted = x.value;
	valuesInPutted = valuesInPutted.split("][");
	valuesInPutted[0] = valuesInPutted[0].replace("[", "");
	valuesInPutted[valuesInPutted.length-1] = valuesInPutted[valuesInPutted.length-1].replace("]", ""); 
	var toUse = new Array();
	for(i=0; i<totInputted; i++){
		toUse[i] = valuesInPutted[i];
	}
	var toUseV = "["+toUse.join("][")+"]";
	totInputted++;
	if(check != valuesInPutted[valuesInPutted.length-1]){
		x.value = toUseV + "["+check+"]";
	}
}

function saveBtn(urlPath){
	var toSave = document.getElementById("lastValueHolder2");
	var compare = document.getElementById("lastValueHolder");
	var valueToSave = document.getElementById("id"+toSave.value).value;
	var expiryDate = document.getElementById("expiryDate").value;

	//s(valueToSave, toSave.value, 0);

	var customerID = document.getElementById("customerID").value;
	var year = document.getElementById("year").value;
	var month = document.getElementById("month").value;	
	var userID = document.getElementById("userID").value;	

	var peak = document.getElementById("peak").value;
	var shoulder = document.getElementById("shoulder").value;
	var offPeak = document.getElementById("offPeak").value;	

	var firstname1 = document.getElementById("firstname1").value;
	var othername1 = document.getElementById("othername1").value;
	var date1 = document.getElementById("date1").value;	
	var time1 = document.getElementById("time1").value;		

	var firstname2 = document.getElementById("firstname2").value;
	var othername2 = document.getElementById("othername2").value;
	var date2 = document.getElementById("date2").value;	
	var time2 = document.getElementById("time2").value;		

	var firstname3 = document.getElementById("firstname3").value;
	var othername3 = document.getElementById("othername3").value;
	var date3 = document.getElementById("date3").value;	
	var time3 = document.getElementById("time3").value;	

	var col = (toSave.value%10);
	var row = parseInt(toSave.value/10);
	var totalSum = 0.0;
	for(i=1; i<=9; i++){
		totalSum +=parseFloat(_("id"+row+i).value.replace(",",""));
	}
	totalSum = totalSum.toFixed(4);

	var meteringPoint = document.getElementById("mp"+row).value;

	var saveID = document.getElementById("saveID");
	saveID.innerHTML = '<i class="fa fa-fw fa-check"></i> Saved';

	var valuesToSend = "user_id="+userID+"&year="+year+"&month="+month+"&customer_id="+customerID+"&metering_point="+meteringPoint+"&col="+col+"&row="+row+"&value="+valueToSave+"&tot="+totalSum+"&peak="+peak+"&shoulder="+shoulder+"&off_peak="+offPeak+"&firstname1="+firstname1+"&othername1="+othername1+"&date1="+date1+"&time1="+time1+"&firstname2="+firstname2+"&othername2="+othername2+"&date2="+date2+"&time2="+time2+"&firstname3="+firstname3+"&othername3="+othername3+"&date3="+date3+"&time3="+time3+"&expiry_date="+expiryDate;
	//	_("toSave").value = valuesToSend;

	var request = new XMLHttpRequest();
	request.open("POST", urlPath+"ajax/saveReadings.php", true);
	request.setRequestHeader("Content-type", "application/x-www-form-urlencoded");			
	request.onreadystatechange = function() {
		if(request.readyState == 4 && request.status == 200) {

			//if(request.responseText != null || request.responseText != "") alert(request.responseText);
			//4    = The connection is complete, the data was sent or retrieved.
			//document.getElementById("id"+toSave.value).value = request.responseText;	
			document.getElementById("id"+toSave.value).style.fontWeight = "700";		
		}
	}
	request.send(valuesToSend);
	
}

function saveReading(urlPath, reading){

	var lastValueHolder = document.getElementById("lastValueHolder");		

	var lastValueHolder2 = document.getElementById("lastValueHolder2");
	var readingValue = document.getElementById("id"+reading).value;	
	var customerID = document.getElementById("customerID").value;
	var year = document.getElementById("year").value;
	var month = document.getElementById("month").value;	
	var userID = document.getElementById("userID").value;
	var expiryDate = document.getElementById("expiryDate").value;

	var peak = document.getElementById("peak").value;
	var shoulder = document.getElementById("shoulder").value;
	var offPeak = document.getElementById("offPeak").value;	

	var firstname1 = document.getElementById("firstname1").value;
	var othername1 = document.getElementById("othername1").value;
	var date1 = document.getElementById("date1").value;	
	var time1 = document.getElementById("time1").value;		

	var firstname2 = document.getElementById("firstname2").value;
	var othername2 = document.getElementById("othername2").value;
	var date2 = document.getElementById("date2").value;	
	var time2 = document.getElementById("time2").value;		

	var firstname3 = document.getElementById("firstname3").value;
	var othername3 = document.getElementById("othername3").value;
	var date3 = document.getElementById("date3").value;	
	var time3 = document.getElementById("time3").value;	

	var saveID = document.getElementById("saveID");
	saveID.innerHTML = '<i class="fa fa-fw fa-save"></i> Save';

	lastValueHolder.value = lastValueHolder2.value; //readingValue;
	lastValueHolder2.value = reading;

	var col = (lastValueHolder.value%10);
	var row = parseInt(lastValueHolder.value/10);
	var totalSum = 0.0;
	for(i=1; i<=9; i++){
		totalSum +=parseFloat(_("id"+row+i).value.replace(",",""));
	}
	totalSum = totalSum.toFixed(4);

	var toSave2 = document.getElementById("id"+lastValueHolder.value).value;
	
	//s(toSave2, lastValueHolder.value, 0);

	var meteringPoint = document.getElementById("mp"+row).value;	

	var valuesToSend = "user_id="+userID+"&year="+year+"&month="+month+"&customer_id="+customerID+"&metering_point="+meteringPoint+"&col="+col+"&row="+row+"&value="+toSave2+"&tot="+totalSum+"&peak="+peak+"&shoulder="+shoulder+"&off_peak="+offPeak+"&firstname1="+firstname1+"&othername1="+othername1+"&date1="+date1+"&time1="+time1+"&firstname2="+firstname2+"&othername2="+othername2+"&date2="+date2+"&time2="+time2+"&firstname3="+firstname3+"&othername3="+othername3+"&date3="+date3+"&time3="+time3+"&expiry_date="+expiryDate;
	//	_("toSave").value = valuesToSend;

	var request = new XMLHttpRequest();
	request.open("POST", urlPath+"ajax/saveReadings.php", true);
	request.setRequestHeader("Content-type", "application/x-www-form-urlencoded");			
	request.onreadystatechange = function() {
		if(request.readyState == 4 && request.status == 200) {
			//4    = The connection is complete, the data was sent or retrieved.
			//alert(request.responseText);
			_("toSave").value = request.responseText;	
			document.getElementById("firstname12").innerHTML = request.responseText;
			//alert(request.responseText);
			document.getElementById("id"+lastValueHolder.value).style.fontWeight = "700";		
		}
	}
	request.send(valuesToSend);
	
}
function lock(urlPath, customerID, year, month){

	var toLock = document.getElementById("id"+customerID+month);
	var valuesToSend = "year="+year+"&month="+month+"&customer_id="+customerID;

	var request = new XMLHttpRequest();
	request.open("POST", urlPath+"ajax/lock.php", true);
	request.setRequestHeader("Content-type", "application/x-www-form-urlencoded");			
	request.onreadystatechange = function() {
		if(request.readyState == 4 && request.status == 200) {
			//4    = The connection is complete, the data was sent or retrieved.
			//document.getElementById("id"+toSave.value).value = request.responseText;	
			toLock.innerHTML = request.responseText;		
		}
	}
	request.send(valuesToSend);	
}


function saveBtnCN(urlPath){
	var toSave = document.getElementById("lastValueHolder2");
	var compare = document.getElementById("lastValueHolder");
	var valueToSave = document.getElementById("id"+toSave.value).value;

	var customerID = document.getElementById("customerID").value;
	var year = document.getElementById("year").value;
	var month = document.getElementById("month").value;	
	var userID = document.getElementById("userID").value;	
	var expiryDate = document.getElementById("expiryDate").value;

	var narration = document.getElementById("narration").value;
	var peak = document.getElementById("peak").value;
	var shoulder = document.getElementById("shoulder").value;
	var offPeak = document.getElementById("offPeak").value;
	var noteDetails = "";//document.getElementById("noteDetails").value;

	var col = (toSave.value%10);
	var row = parseInt(toSave.value/10);
	var totalSum = 0.0;
	for(i=1; i<=9; i++){
		totalSum +=parseFloat(_("id"+row+i).value.replace(",",""));
	}
	totalSum = totalSum.toFixed(4);

	var meteringPoint = document.getElementById("mp"+row).value;

	var saveID = document.getElementById("saveID");
	saveID.innerHTML = '<i class="fa fa-fw fa-check"></i> Saved';

	var valuesToSend = "user_id="+userID+"&year="+year+"&month="+month+"&customer_id="+customerID+"&metering_point="+meteringPoint+"&col="+col+"&row="+row+"&value="+valueToSave+"&tot="+totalSum+"&peak="+peak+"&shoulder="+shoulder+"&off_peak="+offPeak+"&note_details="+noteDetails+"&expiry_date="+expiryDate+"&narration="+narration;
	//	_("toSave").value = valuesToSend;

	var request = new XMLHttpRequest();
	request.open("POST", urlPath+"ajax/saveReadingsCN.php", true);
	request.setRequestHeader("Content-type", "application/x-www-form-urlencoded");			
	request.onreadystatechange = function() {
		if(request.readyState == 4 && request.status == 200) {
			//4    = The connection is complete, the data was sent or retrieved.
			//document.getElementById("id"+toSave.value).value = request.responseText;	
			document.getElementById("id"+toSave.value).style.fontWeight = "700";		
		}
	}
	request.send(valuesToSend);
	
}

function saveBtnCNPrevious(urlPath){
	var toSave = document.getElementById("lastValueHolder2");
	var compare = document.getElementById("lastValueHolder");
	var valueToSave = document.getElementById("id"+toSave.value).value;

	var customerID = document.getElementById("customerID").value;
	var year = document.getElementById("year").value;
	var month = document.getElementById("month").value;	
	var userID = document.getElementById("userID").value;	
	var expiryDate = document.getElementById("expiryDate").value;

	var peak = document.getElementById("peak").value;
	var shoulder = document.getElementById("shoulder").value;
	var offPeak = document.getElementById("offPeak").value;
	var noteDetails = "";//document.getElementById("noteDetails").value;

	var col = (toSave.value%10);
	var row = parseInt(toSave.value/10);
	var totalSum = 0.0;
	for(i=1; i<=9; i++){
		totalSum +=parseFloat(_("id"+row+i).value.replace(",",""));
	}
	totalSum = totalSum.toFixed(4);

	var meteringPoint = document.getElementById("mp"+row).value;

	var saveID = document.getElementById("saveID");
	saveID.innerHTML = '<i class="fa fa-fw fa-check"></i> Saved';

	var valuesToSend = "user_id="+userID+"&year="+year+"&month="+month+"&customer_id="+customerID+"&metering_point="+meteringPoint+"&col="+col+"&row="+row+"&value="+valueToSave+"&tot="+totalSum+"&peak="+peak+"&shoulder="+shoulder+"&off_peak="+offPeak+"&note_details="+noteDetails+"&expiry_date="+expiryDate;
	//	_("toSave").value = valuesToSend;

	var request = new XMLHttpRequest();
	request.open("POST", urlPath+"ajax/saveReadingsCNPrevious.php", true);
	request.setRequestHeader("Content-type", "application/x-www-form-urlencoded");			
	request.onreadystatechange = function() {
		if(request.readyState == 4 && request.status == 200) {
			//4    = The connection is complete, the data was sent or retrieved.
			//document.getElementById("id"+toSave.value).value = request.responseText;	
			document.getElementById("id"+toSave.value).style.fontWeight = "700";		
		}
	}
	request.send(valuesToSend);
	
}

function saveReadingCN(urlPath, reading){

	var lastValueHolder = document.getElementById("lastValueHolder");		

	var lastValueHolder2 = document.getElementById("lastValueHolder2");
	var readingValue = document.getElementById("id"+reading).value;	
	var customerID = document.getElementById("customerID").value;
	var year = document.getElementById("year").value;
	var month = document.getElementById("month").value;	
	var userID = document.getElementById("userID").value;
	var expiryDate = document.getElementById("expiryDate").value;	

	var narration = document.getElementById("narration").value;
	var peak = document.getElementById("peak").value;
	var shoulder = document.getElementById("shoulder").value;
	var offPeak = document.getElementById("offPeak").value;
	var noteDetails = "";//document.getElementById("noteDetails").value;

	var saveID = document.getElementById("saveID");
	saveID.innerHTML = '<i class="fa fa-fw fa-save"></i> Save';

	lastValueHolder.value = lastValueHolder2.value; //readingValue;
	lastValueHolder2.value = reading;

	var col = (lastValueHolder.value%10);
	var row = parseInt(lastValueHolder.value/10);
	var totalSum = 0.0;
	for(i=1; i<=6; i++){
		totalSum +=parseFloat(_("id"+row+i).value.replace(",",""));
	}
	totalSum = totalSum.toFixed(4);

	var toSave2 = document.getElementById("id"+lastValueHolder.value).value;	
	var meteringPoint = document.getElementById("mp"+row).value;	

	var valuesToSend = "user_id="+userID+"&year="+year+"&month="+month+"&customer_id="+customerID+"&metering_point="+meteringPoint+"&col="+col+"&row="+row+"&value="+toSave2+"&tot="+totalSum+"&peak="+peak+"&shoulder="+shoulder+"&off_peak="+offPeak+"&note_details="+noteDetails+"&expiry_date="+expiryDate+"&narration="+narration;
	//	_("toSave").value = valuesToSend;

	var request = new XMLHttpRequest();
	request.open("POST", urlPath+"ajax/saveReadingsCN.php", true);
	request.setRequestHeader("Content-type", "application/x-www-form-urlencoded");			
	request.onreadystatechange = function() {
		if(request.readyState == 4 && request.status == 200) {
			//4    = The connection is complete, the data was sent or retrieved.
			_("toSave").value = request.responseText;	
			document.getElementById("id"+lastValueHolder.value).style.fontWeight = "700";		
		}
	}
	request.send(valuesToSend);
	
}


function saveReadingCNPrevious(urlPath, reading){

	var lastValueHolder = document.getElementById("lastValueHolder");		

	var lastValueHolder2 = document.getElementById("lastValueHolder2");
	var readingValue = document.getElementById("id"+reading).value;	
	var customerID = document.getElementById("customerID").value;
	var year = document.getElementById("year").value;
	var month = document.getElementById("month").value;	
	var userID = document.getElementById("userID").value;
	var expiryDate = document.getElementById("expiryDate").value;	

	var peak = document.getElementById("peak").value;
	var shoulder = document.getElementById("shoulder").value;
	var offPeak = document.getElementById("offPeak").value;
	var noteDetails = "";//document.getElementById("noteDetails").value;

	var saveID = document.getElementById("saveID");
	saveID.innerHTML = '<i class="fa fa-fw fa-save"></i> Save';

	lastValueHolder.value = lastValueHolder2.value; //readingValue;
	lastValueHolder2.value = reading;

	var col = (lastValueHolder.value%10);
	var row = parseInt(lastValueHolder.value/10);
	var totalSum = 0.0;
	for(i=1; i<=6; i++){
		totalSum +=parseFloat(_("id"+row+i).value.replace(",",""));
	}
	totalSum = totalSum.toFixed(4);

	var toSave2 = document.getElementById("id"+lastValueHolder.value).value;	
	var meteringPoint = document.getElementById("mp"+row).value;	

	var valuesToSend = "user_id="+userID+"&year="+year+"&month="+month+"&customer_id="+customerID+"&metering_point="+meteringPoint+"&col="+col+"&row="+row+"&value="+toSave2+"&tot="+totalSum+"&peak="+peak+"&shoulder="+shoulder+"&off_peak="+offPeak+"&note_details="+noteDetails+"&expiry_date="+expiryDate;
	//	_("toSave").value = valuesToSend;

	var request = new XMLHttpRequest();
	request.open("POST", urlPath+"ajax/saveReadingsCNPrevious.php", true);
	request.setRequestHeader("Content-type", "application/x-www-form-urlencoded");			
	request.onreadystatechange = function() {
		if(request.readyState == 4 && request.status == 200) {
			//4    = The connection is complete, the data was sent or retrieved.
			_("toSave").value = request.responseText;	
			document.getElementById("id"+lastValueHolder.value).style.fontWeight = "700";		
		}
	}
	request.send(valuesToSend);
	
}


function clearAll(){
	var allRows = document.getElementById("allRows").value;
	var allColumns = 9;
	var i=j=0;
	for(i=1; i<=allRows; i++){
		for(j=1; j<=allColumns; j++){
			document.getElementById("id"+i+j).value = "0";
		}
	}
}

function clearRow(i){
	var allColumns = 9;
	var j=0;
	for(j=1; j<=allColumns; j++){
		document.getElementById("id"+i+j).value = "0";
	}
	
}

function doSearch() {
    var searchText = document.getElementById('searchTerm').value;
	searchText = searchText.toUpperCase();
    var targetTable = document.getElementById('dataTable');
    var targetTableColCount;
    //Loop through table rows
    for (var rowIndex = 0; rowIndex < targetTable.rows.length; rowIndex++) {
        var rowData = '';

        //Get column count from header row
        if (rowIndex == 0) {
           targetTableColCount = targetTable.rows.item(rowIndex).cells.length;
           continue; //do not execute further code for header row.
        }
                
        //Process data rows. (rowIndex >= 1)
        for (var colIndex = 0; colIndex < targetTableColCount; colIndex++) {
            rowData += targetTable.rows.item(rowIndex).cells.item(colIndex).textContent;
        }

        //If search term is not found in row data
        //then hide the row, else show
        if (rowData.indexOf(searchText) == -1)
            targetTable.rows.item(rowIndex).style.display = 'none';
        else
            targetTable.rows.item(rowIndex).style.display = 'table-row';
    }
}
function doSearch2() {
    var searchText = document.getElementById('searchTerm').value;
	searchText = searchText.toUpperCase();
    var targetTable = document.getElementById('table2');
    var targetTableColCount;
    //Loop through table rows
    for (var rowIndex = 0; rowIndex < targetTable.rows.length; rowIndex++) {
        var rowData = '';

        //Get column count from header row
        if (rowIndex == 0) {
           targetTableColCount = targetTable.rows.item(rowIndex).cells.length;
           continue; //do not execute further code for header row.
        }
                
        //Process data rows. (rowIndex >= 1)
        for (var colIndex = 0; colIndex < targetTableColCount; colIndex++) {
            rowData += targetTable.rows.item(rowIndex).cells.item(colIndex).textContent;
        }

        //If search term is not found in row data
        //then hide the row, else show
        if (rowData.indexOf(searchText) == -1)
            targetTable.rows.item(rowIndex).style.display = 'none';
        else
            targetTable.rows.item(rowIndex).style.display = 'table-row';
    }
}

function multipleChecker(){
	var allboxes = document.getElementsByClassName('AllCheckBoxes');

	var i=0;
	var checker = document.getElementById("checker");

	for(i=0; i<allboxes.length; i++){
		if(checker.innerHTML == "Check All"){
			allboxes[i].checked = true; 
		}else{			
			allboxes[i].checked = false; 
		}
	}

	if(checker.innerHTML == "Check All"){
		checker.innerHTML = "Uncheck All";
	}else{
		checker.innerHTML = "Check All";
	}
}

$('input.number').keyup(function(event) {

  // skip for arrow keys
  if(event.which >= 37 && event.which <= 40) return;

  // format number
  $(this).val(function(index, value) {
  	value = value.replace(/(?!-)[^0-9.]/g, "");
  	valueArray = value.split('.');
  	if(valueArray.length >= 2 ){
  		return valueArray[0].replace(/\B(?=(\d{3})+(?!\d))/g, ",")+'.'+valueArray[1];
  	}else{
  		return valueArray[0].replace(/\B(?=(\d{3})+(?!\d))/g, ",")
  	} 

    return value[0].replace(/\B(?=(\d{3})+(?!\d))/g, ",")
    ;
  });
});

function multipleCheckerLine(number){
	var allboxes = document.getElementsByClassName('AllCheckBoxesLine'+number);

	var i=0;
	var checker = document.getElementById("checkerLine"+number);

	for(i=0; i<allboxes.length; i++){
		if(checker.innerHTML == "C"){
			allboxes[i].checked = true; 
		}else{			
			allboxes[i].checked = false; 
		}
	}

	if(checker.innerHTML == "C"){
		checker.innerHTML = "U";
	}else{
		checker.innerHTML = "C";
	}
}

function multipleCheckerColumn(number){
	var allboxes = document.getElementsByClassName('AllCheckBoxesColumn'+number);

	var i=0;
	var checker = document.getElementById("checkerColumn"+number);

	for(i=0; i<allboxes.length; i++){
		if(checker.innerHTML == "C"){
			allboxes[i].checked = true; 
		}else{			
			allboxes[i].checked = false; 
		}
	}

	if(checker.innerHTML == "C"){
		checker.innerHTML = "U";
	}else{
		checker.innerHTML = "C";
	}
}