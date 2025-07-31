<?php
Class FormulaeBuilder extends BeforeAndAfter{

	public $set_class = "";
	public $set_method = "";
	public $set_customer = "";
	public $set_year = "";
	public $set_month = "";
	public $set_note = "";

	public function parameters($href){
		$portion = explode('/',str_replace(return_url(), '', $href));

		$this->set_class = $portion[0];
		$this->set_method = $portion[1];
		$this->set_customer = $portion[2];
		$this->set_year = $portion[3];
		$this->set_month = $portion[4];
		$this->set_note = $portion[5];
	}


	public function formulae(){

		$class = ($this->set_class)?$this->set_class:portion(1);
		$method = ($this->set_method)?$this->set_method:portion(2);
		$id = ($this->set_customer)?$this->set_customer:portion(3);
		$year = ($this->set_year)?$this->set_year: portion(4);
		$month = ($this->set_month)?$this->set_month: portion(5);

		$note = ($this->set_note)?$this->set_note: portion(6);

		$year_uploaded = $year;
		$month_uploaded = $month;

		if(empty($year) || empty($month) || empty($id)){
			Feedback::error("Please select Customer, Year and Month");
			$units = $this->customerAndSchedule($id, $year, $month);
			$this->energyLinks($id, $year, $month);
			$units = 0.001;
			echo '<div class="clearfix"></div>';
		}else{
			//Feedback::error("No readings found");
			$units = $this->customerAndSchedule($id, $year, $month);
			$this->energyLinks($id, $year, $month);
			$units = 0.001;
			echo '<div class="clearfix"></div>';
			echo '</br>';
		}

	$db = new Db();

	//if(!$this->isLocked($id, $year, $month))
	if(1){

	if($id == 2029){

		if(isset($_POST['test'])){
			$generator = $_POST['generator'];
			$year_uploaded = $_POST['year'];
			$month_uploaded = $_POST['month'];
			$formulae = $_POST['formulae'];
			$narration = $_POST['narration'];
			$workspace = $_POST['workspace'];


		}
		//TANESCO AND SNEL
	echo '<form action="" method="post">';
	echo '<div style="border:1px solid #000; padding:10px;">';
	echo '<div class="row">';
	echo '<div class="col-md-6" id="sideA" '.$display1.'>';
	echo '<div class="col-md-12">';

	echo '<b class="text-primary">WORKSPACE</b>';
	echo '<div class="row" style="margin-bottom:10px;">';
	
	echo '<div class="col-md-6">';
	echo '<b>Generator:</b><br/>';
	$select = $db->select("SELECT * FROM generators ORDER BY ge_name ASC");
	echo '<select style="width:100%;" name="generator" id="generator">';
	echo '<option value=""> Select </option>';
	foreach($select[0] as $row){
		extract($row);
		if($generator == $ge_id)
			echo '<option selected = "selected" value="'.$ge_id.'">'.$ge_name.'</option>';
		else
			echo '<option value="'.$ge_id.'">'.$ge_name.'</option>';
	}
	echo '</select>';
	echo '</div>';
	
	echo '<div class="col-md-2">';
	echo '<b>Year:</b> <select id="year" name="year" style="width:100%;">';
	for($i=2010; $i<=date('Y'); $i++){
		if($i == date('Y') && empty($year_uploaded))
			echo '<option value="'.$i.'" selected="selected">'.($i).'</option>';
		elseif($i == $year_uploaded)
			echo '<option value="'.$i.'" selected="selected">'.($i).'</option>';
		else
			echo '<option value="'.$i.'">'.($i).'</option>';
	}
	echo '</select>';
	echo '</div>';

	echo '<div class="col-md-2">';
	echo '<b>Month:</b> <select id="month" name="month" style="width:100%;">';
	for($i=1; $i<=12; $i++){
		if($i == date('m') && empty($month_uploaded))
			echo '<option value="'.$i.'" selected="selected">'.month_name($i).'</option>';
		elseif($i == $month_uploaded)
			echo '<option value="'.$i.'" selected="selected">'.month_name($i).'</option>';
		else
			echo '<option value="'.$i.'">'.month_name($i).'</option>';
	}
	echo '</select>';
	echo '</div>';
	echo '</div>';
	echo '</div>';

	echo '<div class="col-md-12">';

	echo '<div class="row">';
	echo '<div class="col-md-12">';
	echo '<label for="imports">Formulae</label>';
	echo '<textarea name="workspace" id="workspace" autofocus spellcheck="false" style="font-size:14px;width:100%;height:50px;">'.$workspace.'</textarea>';
	

	echo '<b>Formulae Fields</b><br/>';

	echo '<input id="units" type="button" class="operators" value="([Units])" style="margin-right:5px"/>';
	echo '<input id="usd" type="button" class="operators" value="([Bill_Usd])" style="margin-right:5px"/>';
	echo '<input id="ugx" type="button" class="operators" value="([Bill_Ugx])" style="margin-right:5px"/>';
	echo '<input id="eur" type="button" class="operators" value="([Bill_Eur])" style="margin-right:5px"/>';
	echo '<span style="margin-left:30px;"></span>';
	echo '<input id="ex_eur" type="button" class="operators" value="([Ex_Eur])" style="margin-right:5px"/>';
	echo '<input id="ex_usd" type="button" class="operators" value="([Ex_Usd])" style="margin-right:5px"/>';


	echo '</div>';
	echo '</div>';

	echo '<b>Formulae Operators</b><br/>';
	// echo '<input type="button" class="operators" value="="/>&nbsp;';
	echo '<input type="button" class="operators" value="+"/>&nbsp;';
	echo '<input type="button" class="operators" value="-"/>&nbsp;';
	echo '<input type="button" class="operators" value="*"/>&nbsp;';
	echo '<input type="button" class="operators" value="/"/>&nbsp;';
	echo '&nbsp;&nbsp;&nbsp;';echo '&nbsp;&nbsp;&nbsp;';

	echo '<input type="button" class="operators" value="."/>&nbsp;';
	for($i=0; $i<=9; $i++){
		echo '<input type="button" class="operators" value="'.$i.'"/>&nbsp;';
	}

	//echo '<input type="button" class="operators" value="([DONOT_DISPLAY])"/>&nbsp;';
	// echo '<input type="button" class="operators" value=")"/>&nbsp;';


	echo '<div class="" style="margin-top:10px;">';
	echo '<label for="exports">Formulae Narration</label>';
	echo '<textarea name="narration" id="narration" autofocus spellcheck="false" style="font-size:12px;width:100%;height:50px;">'.$narration.'</textarea>';
	echo '</div>';

	echo '<br/>';
	echo '<button type="submit" name="test" id="test"><i class="fa fa-fw fa-circle"></i> Test</button>&nbsp;';
	echo '<button type="button" id="clear"><i class="fa fa-fw fa-times"></i> Clear</button>&nbsp;';

	echo '</div>';
	
	
	echo '</div>';


	//results
	echo '<div>';
	echo '<h5 class="text-primary">RESULTS</h5>';
	echo '<div id="results">';
	if((isset($_POST['test']) && $_POST['workspace'] != "") || $_POST['workspace'] != ""){

		if(isset($_POST['save'])){
			$generator = $_POST['generator'];
			$year_uploaded = $_POST['year'];
			$month_uploaded = $_POST['month'];
			$formulae = $_POST['formulae'];
			$narration = $_POST['narration'];
			$workspace = $_POST['workspace'];
		}

		$colors = array("blue", "green", "red", "green", "black", "orange", "purple",);
				
		$gname = $this->rgf("generators", $generator, "ge_id", "ge_name");
		echo $gname;
		echo ' = ';
		echo $workspace;
		echo '<br/>';
		// echo $workspace;
		// echo '<br/>';

		$this->get_bill_price($generator, $year_uploaded, $month_uploaded, $workspace);

		
	}

	echo '</div>';
	echo '</div>';

	echo '</div>';
	echo '<div class="clearfix"></div>';

	echo '</div>';
	echo '</form>';
	?>
	<script type="text/javascript">
		$.fn.extend({
		  insertAtCaret: function(myValue) {
		    this.each(function() {
		      if (document.selection) {
		        this.focus();
		        var sel = document.selection.createRange();
		        sel.text = myValue;
		        this.focus();
		      } else if (this.selectionStart || this.selectionStart == '0') {
		        var startPos = this.selectionStart;
		        var endPos = this.selectionEnd;
		        var scrollTop = this.scrollTop;
		        this.value = this.value.substring(0, startPos) +
		          myValue + this.value.substring(endPos,this.value.length);
		        this.focus();
		        this.selectionStart = startPos + myValue.length;
		        this.selectionEnd = startPos + myValue.length;
		        this.scrollTop = scrollTop;
		      } else {
		        this.value += myValue;
		        this.focus();
		      }
		    });
		    return this;
		  }
		});

		$('.operators').click(function(){
			$('#workspace').insertAtCaret($(this).val());
		});

		$('#generator').change(function(){
			var generator = $('#generator').val();
			var year = $('#year').val();
			var month = $('#month').val();

			var form_data = new FormData(); 
			form_data.append('generator', generator); 
			form_data.append('year', year); 
			form_data.append('month', month); 

			$.ajax({
	            url: '<?php echo return_url().'classes/generator-checker.php'; ?>',
	            type: "POST",
	            data: form_data,
	            contentType: false,
	            cache: false,
	            processData:false,
	            success: function(data){
	                var d = JSON.parse(data);
	                $('#narration').val(d.narration); 
	                $('#workspace').val(d.workspace);	
	                $('#fields').html(d.fields);
	                $('#results').html("");
	                
	                if(d.eur){ $('#eur').show(); }else{ $('#eur').hide(); }
	                if(d.ugx){ $('#ugx').show(); }else{ $('#ugx').hide(); }
	                if(d.usd){ $('#usd').show(); }else{ $('#usd').hide(); }
	                if(d.ex_usd){ $('#ex_usd').show(); }else{ $('#ex_usd').hide(); }
	                if(d.ex_eur){ $('#ex_eur').show(); }else{ $('#ex_eur').hide(); }
	            }
	        });
		});

		$('#year').change(function(){
			var generator = $('#generator').val();
			var year = $('#year').val();
			var month = $('#month').val();

			var form_data = new FormData(); 
			form_data.append('generator', generator); 
			form_data.append('year', year); 
			form_data.append('month', month); 

			$.ajax({
	            url: '<?php echo return_url().'classes/generator-checker.php'; ?>',
	            type: "POST",
	            data: form_data,
	            contentType: false,
	            cache: false,
	            processData:false,
	            success: function(data){
	                var d = JSON.parse(data);
	                $('#narration').val(d.narration); 
	                $('#workspace').val(d.workspace);
	                $('#fields').html(d.fields);
	                $('#results').html("");

	                if(d.eur){ $('#eur').show(); }else{ $('#eur').hide(); }
	                if(d.ugx){ $('#ugx').show(); }else{ $('#ugx').hide(); }
	                if(d.usd){ $('#usd').show(); }else{ $('#usd').hide(); }
	                if(d.ex_usd){ $('#ex_usd').show(); }else{ $('#ex_usd').hide(); }
	                if(d.ex_eur){ $('#ex_eur').show(); }else{ $('#ex_eur').hide(); }
	            }
	        });
		});

		$('#month').change(function(){
			var generator = $('#generator').val();
			var year = $('#year').val();
			var month = $('#month').val();

			var form_data = new FormData(); 
			form_data.append('generator', generator); 
			form_data.append('year', year); 
			form_data.append('month', month); 

			$.ajax({
	            url: '<?php echo return_url().'classes/generator-checker.php'; ?>',
	            type: "POST",
	            data: form_data,
	            contentType: false,
	            cache: false,
	            processData:false,
	            success: function(data){
	            	console.log(data);
	                var d = JSON.parse(data);
	                $('#narration').val(d.narration); 
	                $('#workspace').val(d.workspace);
	                $('#fields').html(d.fields);	
	                $('#results').html("");
	                
	                if(d.eur){ $('#eur').show(); }else{ $('#eur').hide(); }
	                if(d.ugx){ $('#ugx').show(); }else{ $('#ugx').hide(); }
	                if(d.usd){ $('#usd').show(); }else{ $('#usd').hide(); }
	                if(d.ex_usd){ $('#ex_usd').show(); }else{ $('#ex_usd').hide(); }
	                if(d.ex_eur){ $('#ex_eur').show(); }else{ $('#ex_eur').hide(); }
	            }
	        });
	       
	    });

	    $('#clear').click(function(){
	    	if(confirm("Do you want clear")){
	    		$("#workspace").val("");
	    		$("#narration").val("");
	            $('#results').html("");
	    	}
	    });
	       
		$('#save').click(function(){
			var generator = $('#generator').val();
			var year = $('#year').val();
			var month = $('#month').val();
			var narration = $('#narration').val();
			var workspace = $('#workspace').val();

			var form_data = new FormData(); 
			form_data.append('generator', generator); 
			form_data.append('year', year); 
			form_data.append('month', month); 
			form_data.append('narration', narration); 
			form_data.append('workspace', workspace); 

			$.ajax({
	            url: '<?php echo return_url().'classes/generator-saver.php'; ?>',
	            type: "POST",
	            data: form_data,
	            contentType: false,
	            cache: false,
	            processData:false,
	            success: function(data){
	                $('#saveStatus').html(data);	
	            }
	        });
		});
	</script>

	<?php
	}else{ //other customers
	$x = "";
	echo '<div style="font-size:14px">';
	echo '<form action="" method="post">';
	$display2 = ' style="display:none;" ';
	$display1 = ' style="display:block;" ';
	if(isset($_POST['workspace_'])){
		$wheelingCharge_ = $_POST['wheelingCharge_'];
		$wheelingChargeE_ = $_POST['wheelingChargeE_'];
		$tlf_ = $_POST['tlf_'];
		$customer_mp_id = $_POST['customer_mp_id'];
		$workspace_ = $_POST['workspace_'];
		$workspace_2 = $_POST['workspace_2'];
		$workspace_2 = $_POST['workspace_2'];
		$workspace_2 = $_POST['workspace_2'];
		$year_uploaded = $_POST['year'];
		$month_uploaded = $_POST['month'];
		$narration = $_POST['narration'];
		$newLabel = $_POST['newLabel'];
		$x = $workspace_;
		$x2 = $workspace_2;
		$x3 = $wheelingCharge_;
		$x4 = $wheelingChargeE_;
		$x5 = $tlf_;

		$display1 = ' style="display:none;" ';
		$display2 = ' style="display:block;" ';
	}
	if(isset($_POST['back'])){
		$wheelingCharge_ = $_POST['wwcc'];
		$wheelingChargeE_ = $_POST['wwccE'];
		$tlf_ = $_POST['wwcctlf'];
		$customer_mp_id = $_POST['customer_mp_id'];
		$workspace_ = $_POST['workspace_'];
		$workspace_2 = $_POST['workspace_2'];
		$workspace_2 = $_POST['workspace_2'];
		$workspace_2 = $_POST['workspace_2'];
		$year_uploaded = $_POST['year'];
		$month_uploaded = $_POST['month'];
		$narration = $_POST['narration'];
		$newLabel = $_POST['newLabel'];
		$x = $workspace_;
		$x2 = $workspace_2;
		$x3 = $wheelingCharge_;
		$x4 = $wheelingChargeE_;
		$x5 = $tlf_;

		$display2 = ' style="display:none;" ';
		$display1 = ' style="display:block;" ';
	}

	echo '<div class="col-md-12" style="padding:20px;border:1px solid #000;">';
	echo '<div class="row">';
	echo '<div class="col-md-12">';
	//echo '<h4 class="text-primary"> FORMULAE BUILBER</h4>';
	//echo '<h5>Customer: <span class="text-danger text-bold">'.$this->rgf("customer", $id, "customer_id", "customer_short_name").'</span></h5>';
	echo '</div>';
	//echo '</div>';
	// echo '<div class="col-md-12" style="margin-bottom:20px;">Label:<input type="text" value="" class="form-control"/></div>';
	echo '<div class="col-md-12" id="sideA" '.$display1.'>';
	echo '<div class="col-md-12">';

	echo '<b class="text-primary">WORKSPACE</b>';
	echo '<div class="row" style="margin-bottom:10px;">';
	echo '<div class="col-md-2">';
	echo '<b>Active Part</b><br/>';
	echo '<select style="width:100%;" id="side"><option value="Imports">Imports</option><option value="Exports">Exports</option><option value="wheelingCharge">Imports Wheeling Charge</option><option value="wheelingChargeE">Exports Wheeling Charge</option><option value="tlf">Transmission Loss Factor</option></select>';
	echo '</div>';
	echo '<div class="col-md-3">';
	echo '<b>Metering Point:</b><br/>';
	$select = $db->select("SELECT * FROM metering_point WHERE mp_customer_id = '$id' ORDER BY mp_location ASC");
	echo '<select style="width:100%;" name="customer_mp_id" id="meteringPoint">';
	echo '<option value="">Select</option>';
	foreach($select[0] as $row){
		extract($row);
		if($mp_id == $customer_mp_id)
			echo '<option selected = "selected" value="'.$mp_id.'">'.$mp_location.'</option>';
		else
			echo '<option value="'.$mp_id.'">'.$mp_location.'</option>';
	}
	echo '</select>';
	echo '<input type="hidden" id="customer_id" value="'.$id.'"/>';
	echo '</div>';
	echo '<div class="col-md-3">';
	echo '<b>New Label:</b> <input id="newLabel" name="newLabel" style="width:100%;" type="text" value="'.$newLabel.'"/>';
	echo '</div>';
	echo '<div class="col-md-2">';
	echo '<b>Year:</b> <select id="year" name="year" style="width:100%;">';
	for($i=2010; $i<=date('Y'); $i++){
		if($i == date('Y') && empty($year_uploaded))
			echo '<option value="'.$i.'" selected="selected">'.($i).'</option>';
		elseif($i == $year_uploaded)
			echo '<option value="'.$i.'" selected="selected">'.($i).'</option>';
		else
			echo '<option value="'.$i.'">'.($i).'</option>';
	}
	echo '</select>';
	echo '</div>';

	echo '<div class="col-md-2">';
	echo '<b>Month:</b> <select id="month" name="month" style="width:100%;">';
	for($i=1; $i<=12; $i++){
		if($i == date('m') && empty($month_uploaded))
			echo '<option value="'.$i.'" selected="selected">'.month_name($i).'</option>';
		elseif($i == $month_uploaded)
			echo '<option value="'.$i.'" selected="selected">'.month_name($i).'</option>';
		else
			echo '<option value="'.$i.'">'.month_name($i).'</option>';
	}
	echo '</select>';
	echo '</div>';
	echo '</div>';
	echo '</div>';

	echo '<div class="col-md-9">';

	echo '<div class="row">';
	echo '<div class="col-md-6">';
	echo '<label for="imports">Imports</label>';
	echo '<textarea name="workspace" id="workspace" autofocus spellcheck="false" style="font-size:12px;width:100%;height:60px;">'.$workspace_.'</textarea>';
	echo '<input type="hidden" name="workspace_" value="'.$workspace_.'" id="workspace_"/>';
	echo '<input type="hidden" name="wheelingCharge_" value="'.$wheelingCharge_.'" id="wheelingCharge_"/>';
	echo '<input type="hidden" name="wheelingChargeE_" value="'.$wheelingChargeE_.'" id="wheelingChargeE_"/>';
	echo '<input type="hidden" name="tlf_" value="'.$tlf_.'" id="tlf_"/>';

	echo '<input type="hidden" name="note" value="'.$note.'" id="note"/>';
	echo '</div>';

	echo '<div class="col-md-6">';
	echo '<label for="exports">Exports</label>';
	echo '<textarea name="workspace2" id="workspace2" autofocus spellcheck="false" style="font-size:12px;width:100%;height:60px;">'.$workspace_2.'</textarea>';
	echo '<input type="hidden" name="workspace_2" value="'.$workspace_2.'" id="workspace_2"/>';
	echo '</div>';


	echo '<div class="col-md-6">';
	echo '<b>Imports Wheeling Charge</b><br/>';
	echo '<textarea id="wheelingCharge" name="wheelingCharge" style="width:100%;height:60px;">'.$wheelingCharge_.'</textarea>';
	echo '</div>';


	echo '<div class="col-md-6">';
	echo '<b>Export Wheeling Charge<br/></b>';
	echo '<textarea id="wheelingChargeE" name="wheelingChargeE" style="width:100%;height:60px;">'.$wheelingChargeE_.'</textarea>';
	echo '</div>';

	echo '<div class="col-md-12">';
	echo '<label for="exports">Transmission Loss Factor</label>';
	echo '<textarea name="tlf" id="tlf" autofocus spellcheck="false" style="font-size:12px;width:100%;height:50px;">'.$tlf_.'</textarea>';
	echo '</div>';

	echo '<div class="col-md-12">';
	echo '<label for="exports">Formulae Narration</label>';
	echo '<textarea name="narration" id="narration" autofocus spellcheck="false" style="font-size:12px;width:100%;height:50px;">'.$narration.'</textarea>';
	echo '</div>';

	echo '</div>';

	echo '<b>Formulae Operators</b><br/>';
	// echo '<input type="button" class="operators" value="="/>&nbsp;';
	echo '<input type="button" class="operators" value="+"/>&nbsp;';
	echo '<input type="button" class="operators" value="-"/>&nbsp;';
	echo '<input type="button" class="operators" value="*"/>&nbsp;';
	echo '<input type="button" class="operators" value="/"/>&nbsp;';
	echo '&nbsp;&nbsp;&nbsp;';echo '&nbsp;&nbsp;&nbsp;';
	echo '<input type="button" class="operators" value="."/>&nbsp;';
	echo '<input type="button" class="operators" value="0"/>&nbsp;';
	echo '<input type="button" class="operators" value="1"/>&nbsp;';
	echo '<input type="button" class="operators" value="2"/>&nbsp;';
	echo '<input type="button" class="operators" value="3"/>&nbsp;';
	echo '<input type="button" class="operators" value="4"/>&nbsp;';
	echo '<input type="button" class="operators" value="5"/>&nbsp;';
	echo '<input type="button" class="operators" value="6"/>&nbsp;';
	echo '<input type="button" class="operators" value="7"/>&nbsp;';
	echo '<input type="button" class="operators" value="8"/>&nbsp;';
	echo '<input type="button" class="operators" value="9"/>&nbsp;';
	echo '<input type="button" class="operators" value="([DONOT_DISPLAY])"/>&nbsp;';
	// echo '<input type="button" class="operators" value=")"/>&nbsp;';
	echo '<br/>';
	echo '<br/>';
	echo '<button type="submit" id="test"><i class="fa fa-fw fa-circle"></i> Test</button>&nbsp;';
	echo '<button type="button" id="clear"><i class="fa fa-fw fa-times"></i> Clear</button>&nbsp;';

	echo '</div>';
	echo '<div class="col-md-3" style="padding:0;">';
	
	echo '<b class="">Formulae Fields</b>';
	echo '<div style="margin-bottom:20px;height:270px;overflow-y:scroll;overflow-x:hidden; border:1px solid #000; padding:10px;">';
	$select = $db->select("SELECT * FROM customer ORDER BY customer_short_name ASC");
	?>
	<style type="text/css">
		.customer-menu ul li{display:block; width:100%;}
		.customers .customer-label{
			padding:0 10px;
			margin:2px;
			width:100%;
			cursor:pointer;
		}
		.customers-list{
		}
		.metering-point{
			color:#000;
			background-color:#ccc;
			margin-top:1px;	
			margin-bottom:0px;
			margin-top:0px;		
		}
		.nested{display:none;}
		.active{display:block;}
		.list-style{
			margin-bottom:0px;
			margin-top:0px;
		}
		.customer-name{
			background-color: #9a9a9a;
			color:#fff;
		}
	</style>
	<?php
	echo '<nav>';
	echo '<ul  class="customer-menu" style="font-size:0.9em;padding-left:0;">';
	foreach($select[0] as $row){
		extract($row);
		echo '<li class="customers" style="font-weight:bold;">';
		if($customer_id == $id)
			echo '<p class="customer-label customer-name" style="background-color:#1f91f3;">'.$customer_short_name.'</p>';
		else
			echo '<p class="customer-label customer-name">'.$customer_short_name.'</p>';
		$xy = new Db();
		$xx = $xy->select("SELECT * FROM metering_point WHERE mp_customer_id = '$customer_id' ORDER BY mp_location ASC");
		if($xy->num_rows()){

			echo '<ul class="nested" style="padding-left:25px;">';

			echo '<li class="fields" title="'.$customer_short_name.'" style="cursor:pointer" data-id="=>(['.$customer_short_name.'])"><span style=""></span> =>(['.$customer_short_name.'])</li>';

			foreach($xx[0] as $y){
				extract($y);

				echo '<li class="">';
				echo '<p class="customer-list metering-point customer-label">'.$mp_location.'</p>';
				
				echo '<ul class="list-style nested" style="padding-left:25px;">';
				echo '<li class="fields" title="'.$mp_id.'" style="cursor:pointer" data-id="([Imports@'.$mp_location.'])"><span style="">&#x25cf;</span> Imports</li>';
				echo '<li class="fields" style="cursor:pointer" data-id="([Exports@'.$mp_location.'])"><span style="">&#x25cf;</span> Exports</li>';
				echo '<li class="fields" style="cursor:pointer" data-id="([NetImports@'.$mp_location.'])"><span style="">&#x25cf;</span> NetImports</li>';
				echo '<li class="fields" style="cursor:pointer" data-id="([NetExports@'.$mp_location.'])"><span style="">&#x25cf;</span> NetExports</li>';
				echo '</ul>';

				echo '</li>';
			}
			echo '</ul>';
		}
		echo '</li>';
	}
	echo '</ul>';
	echo '</nav>';
	
	echo '</div>';
	
	echo '</div>';
	echo '</div>';
	echo '</div>';
	echo '</form>';

	echo '<div class="clearfix"></div>';
	echo '<div id="results" '.$display2.'>';
	echo '<div>';
	echo '<b class="text-primary">RESULTS</b>';
	echo '</div>';
	echo '<table style="width:100%;">';
	echo '<tr valign="top">';
	echo '<td style="width:50%">';
	echo '<div style="border:1px solid #000;padding:10px;margin:10px;">';
	
	echo '<b><span style="color:red;">IMPORTS</span><br/>Formula '.$year_uploaded.' '.month_name($month_uploaded).': </b><br/>'.$this->rgf("metering_point", $customer_mp_id, "mp_id", "mp_location").' = '.$x;
	
	$error = array();
	preg_match_all('/\([\w+@[a-z0-9A-Z-\(\).:\s+*-\/]+]([0-9\/+*-\.]+)?\)/',$x,$matches,PREG_PATTERN_ORDER);
	// echo '<pre>'; print_r($matches);
	// echo '</pre>';
	$all_points = $matches;
	$idFormula = $x;
	foreach($all_points[0] as $ap){

		$ap = "@@@@".$ap."@@@@";

		$ap = str_replace('@@@@([', '', $ap);
		//$ap = str_replace(')@@@@', '', $ap);
		$ap = str_replace(']*-', '*-', $ap);
		$ap = str_replace(']*', '*', $ap);
		$ap = str_replace(']/', '/', $ap);
		$ap = str_replace(']-', '-', $ap);
		$ap = str_replace(']+', '+', $ap);
		$ap = str_replace('])@@@@', '', $ap);
		$ap = str_replace(')@@@@', '', $ap);
		
		preg_match_all('/[+\/*-](-)?[0-9\.]+/',$ap,$matches3,PREG_PATTERN_ORDER);				
		$nas = $matches3[0][0];
		
		$ap = str_replace($nas, '', $ap);

		$p = explode('@',$ap);
	
		if(is_array($p)){
			
			$point_id = $this->rgf("metering_point", $p[1], "mp_location", "mp_id");
			if($point_id){
				$customer = $this->rgf("metering_point", $point_id, "mp_id", "mp_customer_id");
				
				if($p[0]=="Imports"){
					$type = "I";
				}elseif($p[0]=="Exports"){
					$type = "E";
				}elseif($p[0]=="NetImports"){
					$type = "NI";
				}elseif($p[0]=="NetExports"){
					$type = "NE";
				}

				
				$idFormula = str_replace('['.$ap.']', '['.$type.'@'.$point_id.']', $idFormula);

				//echo '<br/><b>';
				// echo $this->rgf("customer", $customer, "customer_id", "customer_short_name"); 
				// echo '->';
				// echo $p[1].'=>';
				// echo '</b>';
				// //echo '('.$p[0].')';
				// $r = $this->get_readings($type, $customer, $point_id, $month_uploaded, $year_uploaded);

				// foreach($r as $label => $reading){
				// 	echo '<b>'.$type.$label.': </b>'.number_format($reading/1000).'&nbsp;&nbsp;';
				// }
				//echo '<br/>';
				 
			}else{
				$error[]="Metering Point No found, Check Meter Name";
			}

		}else{
			$error[] = "Invalid Formula, Check Format";
		}
	}
	if(empty($errors)){

	}else{
		echo implode($error);
	}

	if($idFormula){
		//echo '<br/>'.$idFormula.'<br/>';
	echo '<input type="hidden" id="sfe" value="'.$idFormula.'"/>';
		if($note == "credit-note")
			$r = $this->get_readings2($customer, $point_id, $month_uploaded, $year_uploaded, $idFormula, 1);
		else
			$r = $this->get_readings2($customer, $point_id, $month_uploaded, $year_uploaded, $idFormula);
	}
	
	echo '</div>';	
	echo '</td>';
	
	echo '<td>';
	echo '<div style="border:1px solid #000;padding:10px;margin:10px;">';
	$x=$x2;
	echo '<b><span style="color:red;">EXPORTS</span><br/>Formula '.$year_uploaded.' '.month_name($month_uploaded).': </b><br/>'.$this->rgf("metering_point", $customer_mp_id, "mp_id", "mp_location").' = '.$x;

	$error = array();
	preg_match_all('/\([\w+@[a-z0-9A-Z-().:\s+*-\/]+]([0-9\/+*-\.]+)?\)/',$x,$matches,PREG_PATTERN_ORDER);
	// echo '<pre>'; print_r($matches);
	// echo '</pre>';
	$all_points = $matches;
	$idFormula = $x;
	foreach($all_points[0] as $ap){

		$ap = "@@@@".$ap."@@@@";

		$ap = str_replace('@@@@([', '', $ap);
		//$ap = str_replace(')@@@@', '', $ap);
		$ap = str_replace(']*', '*', $ap);
		$ap = str_replace(']/', '/', $ap);
		$ap = str_replace(']-', '-', $ap);
		$ap = str_replace(']+', '+', $ap);
		$ap = str_replace('])@@@@', '', $ap);
		$ap = str_replace(')@@@@', '', $ap);

		preg_match_all('/[+\/*-](-)?[0-9\.]+/',$ap,$matches3,PREG_PATTERN_ORDER);				
		$nas = $matches3[0][0];
		
		//print_r($matches3);
		$ap = str_replace($nas, '', $ap);

		$p = explode('@',$ap);
		if(is_array($p)){
			
			$point_id = $this->rgf("metering_point", $p[1], "mp_location", "mp_id");
			if($point_id){
				$customer = $this->rgf("metering_point", $point_id, "mp_id", "mp_customer_id");
				
				if($p[0]=="Imports"){
					$type = "I";
				}elseif($p[0]=="Exports"){
					$type = "E";
				}elseif($p[0]=="NetImports"){
					$type = "NI";
				}elseif($p[0]=="NetExports"){
					$type = "NE";
				}

				
				$idFormula = str_replace('['.$ap.']', '['.$type.'@'.$point_id.']', $idFormula);
				//echo '<br/><b>';
				// echo $this->rgf("customer", $customer, "customer_id", "customer_short_name"); 
				// echo '->';
				// echo $p[1].'=>';
				// echo '</b>';
				// //echo '('.$p[0].')';
				// $r = $this->get_readings($type, $customer, $point_id, $month_uploaded, $year_uploaded);

				// foreach($r as $label => $reading){
				// 	echo '<b>'.$type.$label.': </b>'.number_format($reading/1000).'&nbsp;&nbsp;';
				// }
				//echo '<br/>';
				 
			}else{
				$error[]="Metering Point No found, Check Meter Name";
			}

		}else{
			$error[] = "Invalid Formula, Check Format";
		}
	}
	if(empty($errors)){

	}else{
		echo implode($error);
	}

	if($idFormula){
		//echo '<br/>'.$idFormula.'<br/>';
		echo '<input type="hidden" id="sfi" value="'.$idFormula.'"/>';
		
		if($note == "credit-note")
			$r = $this->get_readings2($customer, $point_id, $month_uploaded, $year_uploaded, $idFormula, 1);
		else
			$r = $this->get_readings2($customer, $point_id, $month_uploaded, $year_uploaded, $idFormula);
	}
	
	echo '</div>';	
	echo '</td>';	
	echo '</tr>';

	echo '<tr valign="top">';
	echo '<td>';
	echo '<div style="border:1px solid black; min-height:80px;padding:10px;margin:10px;">';
	echo '<p style="color:red; font-weight:bold;">Imports Wheeling Charge: </p>';
	echo $wheelingCharge_;
	echo '<br/>';

	$error = array();
	$x = explode("=>",$wheelingCharge_);

	if(!isset($x[1])){
		$error[] = '<span style="color:red;">Please attach payable customer.</span>';
		$xx = "";
	}else{
		$xx = "=>".$x[1];
	}

	$x = $x[0];
	preg_match_all('/\([\w+@[a-z0-9A-Z-().:\s+*-\/]+]([0-9\/+*-\.]+)?\)/',$x,$matches,PREG_PATTERN_ORDER);
	// echo '<pre>'; print_r($matches);
	// echo '</pre>';
	$all_points = $matches;
	$idFormula = $x;
	foreach($all_points[0] as $ap){

		$ap = "@@@@".$ap."@@@@";

		$ap = str_replace('@@@@([', '', $ap);
		//$ap = str_replace(')@@@@', '', $ap);
		$ap = str_replace(']*', '*', $ap);
		$ap = str_replace(']/', '/', $ap);
		$ap = str_replace(']-', '-', $ap);
		$ap = str_replace(']+', '+', $ap);
		$ap = str_replace('])@@@@', '', $ap);
		$ap = str_replace(')@@@@', '', $ap);

		preg_match_all('/[+\/*-](-)?[0-9\.]+/',$ap,$matches3,PREG_PATTERN_ORDER);				
		$nas = $matches3[0][0];
		
		//print_r($matches3);
		$ap = str_replace($nas, '', $ap);

		$p = explode('@',$ap);
		if(is_array($p)){
			
			$point_id = $this->rgf("metering_point", $p[1], "mp_location", "mp_id");
			if($point_id){
				$customer = $this->rgf("metering_point", $point_id, "mp_id", "mp_customer_id");
				
				if($p[0]=="Imports"){
					$type = "I";
				}elseif($p[0]=="Exports"){
					$type = "E";
				}elseif($p[0]=="NetImports"){
					$type = "NI";
				}elseif($p[0]=="NetExports"){
					$type = "NE";
				}

				//echo '******'.$idFormula.'*****';
				$idFormula = str_replace('['.$ap.']', '['.$type.'@'.$point_id.']', $idFormula);
				//echo '<br/><b>';
				// echo $this->rgf("customer", $customer, "customer_id", "customer_short_name"); 
				// echo '->';
				// echo $p[1].'=>';
				// echo '</b>';
				// //echo '('.$p[0].')';
				// $r = $this->get_readings($type, $customer, $point_id, $month_uploaded, $year_uploaded);

				// foreach($r as $label => $reading){
				// 	echo '<b>'.$type.$label.': </b>'.number_format($reading/1000).'&nbsp;&nbsp;';
				// }
				//echo '<br/>';
				 
			}else{
				$error[]="Metering Point No found, Check Meter Name";
			}

		}else{
			$error[] = "Invalid Formula, Check Format";
		}
	}
	if(empty($error)){

	}else{
		echo implode($error);
	}

	//if($idFormula)
	{
		//echo '<br/>'.$idFormula.'<br/>';
		echo '<input type="hidden" id="wc_sfi" value="'.$idFormula.$xx.'"/>';

		if($note == "credit-note")
			$r = $this->get_readings2($customer, $point_id, $month_uploaded, $year_uploaded, $idFormula, 1);
		else
			$r = $this->get_readings2($customer, $point_id, $month_uploaded, $year_uploaded, $idFormula);
	}
	
	echo '</div>';
	echo '</td>';

	echo '<td>';
	echo '<div style="border:1px solid black; min-height:80px;padding:10px;margin:10px;">';
	echo '<p style="color:red; font-weight:bold;">Exports Wheeling Charge: </p>';
	echo $wheelingChargeE_;

	$error = array();
	$x = explode("=>",$wheelingChargeE_);
	
	if(!isset($x[1])){
		$error[] = '<span style="color:red;">Please attach payable customer.</span>';
		$xx = "";
	}else{
		$xx = "=>".$x[1];
	}
	$x = $x[0];

	preg_match_all('/\([\w+@[a-z0-9A-Z-().:\s+*-\/]+]([0-9\/+*-\.]+)?\)/',$x,$matches,PREG_PATTERN_ORDER);
	// echo '<pre>'; print_r($matches);
	// echo '</pre>';
	$all_points = $matches;
	$idFormula = $x;
	foreach($all_points[0] as $ap){

		$ap = "@@@@".$ap."@@@@";

		$ap = str_replace('@@@@([', '', $ap);
		//$ap = str_replace(')@@@@', '', $ap);
		$ap = str_replace(']*', '*', $ap);
		$ap = str_replace(']/', '/', $ap);
		$ap = str_replace(']-', '-', $ap);
		$ap = str_replace(']+', '+', $ap);
		$ap = str_replace('])@@@@', '', $ap);
		$ap = str_replace(')@@@@', '', $ap);

		preg_match_all('/[+\/*-](-)?[0-9\.]+/',$ap,$matches3,PREG_PATTERN_ORDER);				
		$nas = $matches3[0][0];
		
		//print_r($matches3);
		$ap = str_replace($nas, '', $ap);

		$p = explode('@',$ap);
		if(is_array($p)){
			
			$point_id = $this->rgf("metering_point", $p[1], "mp_location", "mp_id");
			if($point_id){
				$customer = $this->rgf("metering_point", $point_id, "mp_id", "mp_customer_id");
				
				if($p[0]=="Imports"){
					$type = "I";
				}elseif($p[0]=="Exports"){
					$type = "E";
				}elseif($p[0]=="NetImports"){
					$type = "NI";
				}elseif($p[0]=="NetExports"){
					$type = "NE";
				}

				
				$idFormula = str_replace('['.$ap.']', '['.$type.'@'.$point_id.']', $idFormula);
				//echo '<br/><b>';
				// echo $this->rgf("customer", $customer, "customer_id", "customer_short_name"); 
				// echo '->';
				// echo $p[1].'=>';
				// echo '</b>';
				// //echo '('.$p[0].')';
				// $r = $this->get_readings($type, $customer, $point_id, $month_uploaded, $year_uploaded);

				// foreach($r as $label => $reading){
				// 	echo '<b>'.$type.$label.': </b>'.number_format($reading/1000).'&nbsp;&nbsp;';
				// }
				//echo '<br/>';
				 
			}else{
				$error[]="Metering Point No found, Check Meter Name";
			}

		}else{
			$error[] = "Invalid Formula, Check Format";
		}
	}
	if(empty($errors)){

	}else{
		echo implode($error);
	}

	//if($idFormula)
	{
		//echo '<br/>'.$idFormula.'<br/>';

		echo '<input type="hidden" id="wc_sfe" value="'.$idFormula.$xx.'"/>';

		if($note == "credit-note")
			$r = $this->get_readings2($customer, $point_id, $month_uploaded, $year_uploaded, $idFormula, 1);
		else
			$r = $this->get_readings2($customer, $point_id, $month_uploaded, $year_uploaded, $idFormula);
	}
	echo '</div>';
	echo '</td>';
	echo '</tr>';
	
	echo '<tr>';
	echo '<td colspan="2">';
	echo '<div style="border:1px solid black; min-height:80px;padding:10px;margin:10px;">';
	echo '<p style="color:red; font-weight:bold;">Transmission Loss Factor: </p>';
	echo $tlf_;

	
	$x = $tlf_;

	preg_match_all('/\([\w+@[a-z0-9A-Z-().:\s+*-\/]+]([0-9\/+*-\.]+)?\)/',$x,$matches,PREG_PATTERN_ORDER);
	// echo '<pre>'; print_r($matches);
	// echo '</pre>';
	$all_points = $matches;
	$idFormula = $x;
	foreach($all_points[0] as $ap){

		$ap = "@@@@".$ap."@@@@";

		$ap = str_replace('@@@@([', '', $ap);
		//$ap = str_replace(')@@@@', '', $ap);
		$ap = str_replace(']*', '*', $ap);
		$ap = str_replace(']/', '/', $ap);
		$ap = str_replace(']-', '-', $ap);
		$ap = str_replace(']+', '+', $ap);
		$ap = str_replace('])@@@@', '', $ap);
		$ap = str_replace(')@@@@', '', $ap);

		preg_match_all('/[+\/*-](-)?[0-9\.]+/',$ap,$matches3,PREG_PATTERN_ORDER);				
		$nas = $matches3[0][0];
		
		//print_r($matches3);
		$ap = str_replace($nas, '', $ap);

		$p = explode('@',$ap);
		if(is_array($p)){
			
			$point_id = $this->rgf("metering_point", $p[1], "mp_location", "mp_id");
			if($point_id){
				$customer = $this->rgf("metering_point", $point_id, "mp_id", "mp_customer_id");
				
				if($p[0]=="Imports"){
					$type = "I";
				}elseif($p[0]=="Exports"){
					$type = "E";
				}elseif($p[0]=="NetImports"){
					$type = "NI";
				}elseif($p[0]=="NetExports"){
					$type = "NE";
				}

				
				$idFormula = str_replace('['.$ap.']', '['.$type.'@'.$point_id.']', $idFormula);
				//echo '<br/><b>';
				// echo $this->rgf("customer", $customer, "customer_id", "customer_short_name"); 
				// echo '->';
				// echo $p[1].'=>';
				// echo '</b>';
				// //echo '('.$p[0].')';
				// $r = $this->get_readings($type, $customer, $point_id, $month_uploaded, $year_uploaded);

				// foreach($r as $label => $reading){
				// 	echo '<b>'.$type.$label.': </b>'.number_format($reading/1000).'&nbsp;&nbsp;';
				// }
				//echo '<br/>';
				 
			}else{
				$error[]="Metering Point No found, Check Meter Name";
			}

		}else{
			$error[] = "Invalid Formula, Check Format";
		}
	}
	if(empty($errors)){

	}else{
		echo implode($error);
	}

	//if($idFormula)
	{
		//echo '<br/>'.$idFormula.'<br/>';

		echo '<input type="hidden" id="tlfactor" value="'.$idFormula.'"/>';
		
		if($note == "credit-note")
			$r = $this->get_readings2($customer, $point_id, $month_uploaded, $year_uploaded, $idFormula, 1);
		else
			$r = $this->get_readings2($customer, $point_id, $month_uploaded, $year_uploaded, $idFormula);
	}
	echo '</div>';
	echo '</td>';
	echo '</tr>';

	echo '</table>';

	echo '<div class="col-md-12">';
	echo '<br/>';
	echo '<input type="hidden" value="'.$wheelingCharge_.'" name="wwcc"/>';
	echo '<input type="hidden" value="'.$wheelingChargeE_.'" name="wwccE"/>';
	echo '<input type="hidden" value="'.$tlf_.'" name="wwcctlf"/>';
	echo '<button type="submit" name="back" id=""><i class="fa fa-fw fa-arrow-left"></i> Back</button>&nbsp;';
	if($this->isLocked($id, $year_uploaded, $month_uploaded))
	{
		echo '<span class="bg-danger">Formula Builder is Locked.</span>';
	
	}else{
		echo '<button type="button" id="apply"><i class="fa fa-fw fa-check"></i> Apply</button>&nbsp;<span id="applyStatus"></span>';
	}
	
	echo '</div>';
		
	echo '</div>';
	echo '</div>';
	?>

	<script type="text/javascript">
		$('#meteringPoint').change(function(){
			$('#narration').val("Loading...");
			$('#workspace').val("Loading...");
			$('#workspace2').val("Loading...");
			$('#newLabel').val("Loading...");
			$('#wheelingCharge').val("Loading...");
			$('#wheelingChargeE').val("Loading...");
			/////////////////////////////////////
			var year = $('#year').val();
			var month = $('#month').val();
			var customer_id = $('#customer_id').val();
			var mp_id = $("#meteringPoint").val();
			var note = $("#note").val();

			////////////////////////////////////////////
			var form_data = new FormData(); 
			form_data.append('customer_id', customer_id); 
			form_data.append('year', year); 
			form_data.append('month', month); 
			form_data.append('mp_id', mp_id);
			form_data.append('note', note); 

			$.ajax({
	            url: '<?php echo return_url().'classes/formulae-checker.php'; ?>',
	            type: "POST",
	            data: form_data,
	            contentType: false,
	            cache: false,
	            processData:false,
	            success: function(data){
	                
	                var d = JSON.parse(data);
	                $('#narration').val(d.narration); 
	                $('#newLabel').val(d.newLabel);
	                $('#workspace').val(d.workspace); 
	                $('#workspace_').val(d.workspace);
	                $('#workspace2').val(d.workspace2);
	                $('#workspace_2').val(d.workspace2);
	                $('#wheelingCharge').val(d.wheelingCharge);	
	                $('#wheelingCharge_').val(d.wheelingCharge);
	                $('#wheelingChargeE').val(d.wheelingChargeE);	
	                $('#wheelingChargeE_').val(d.wheelingChargeE);
	                $('#tlf').val(d.tlf);
	                $('#tlf_').val(d.tlf);	
	            }
	        });
		});	

		$('#month').change(function(){
			$('#narration').val("Loading...");
			$('#workspace').val("Loading...");
			$('#workspace2').val("Loading...");
			$('#newLabel').val("Loading...");
			$('#wheelingCharge').val("Loading...");
			$('#wheelingChargeE').val("Loading...");
			$('#tlf').val("Loading...");
			/////////////////////////////////////
			var year = $('#year').val();
			var month = $('#month').val();
			var customer_id = $('#customer_id').val();
			var mp_id = $("#meteringPoint").val();
			var note = $("#note").val();
			////////////////////////////////////////////
			var form_data = new FormData(); 
			form_data.append('customer_id', customer_id); 
			form_data.append('year', year); 
			form_data.append('month', month); 
			form_data.append('mp_id', mp_id);
			form_data.append('note', note); 

			$.ajax({
	            url: '<?php echo return_url().'classes/formulae-checker.php'; ?>',
	            type: "POST",
	            data: form_data,
	            contentType: false,
	            cache: false,
	            processData:false,
	            success: function(data){
	                
	                var d = JSON.parse(data);
	                $('#narration').val(d.narration); 
	                $('#newLabel').val(d.newLabel);
	                $('#workspace').val(d.workspace); 
	                $('#workspace_').val(d.workspace);
	                $('#workspace2').val(d.workspace2);
	                $('#workspace_2').val(d.workspace2);
	                $('#wheelingCharge').val(d.wheelingCharge);	
	                $('#wheelingCharge_').val(d.wheelingCharge);
	                $('#wheelingChargeE').val(d.wheelingChargeE);	
	                $('#wheelingChargeE_').val(d.wheelingChargeE);	
					$('#tlf').val(d.tlf);
					$('#tlf_').val(d.tlf);
	            }
	        });
		});	
		
		$('#apply').click(function(){
			var narration = $('#narration').val();
			var workspace = $('#workspace').val();
			var workspace2 = $('#workspace2').val();
			var sfi = $('#sfi').val();
			var sfe = $('#sfe').val();
			var wc_sfi = $('#wc_sfi').val();
			var wc_sfe = $('#wc_sfe').val();
			var newLabel = $('#newLabel').val();
			var wheelingCharge = $('#wheelingCharge').val();
			var wheelingChargeE = $('#wheelingChargeE').val();
			var tlf = $('#tlf').val();
			var tlfactor = $('#tlfactor').val();
			var note = $('#note').val();
			///////////////////////////////////
			var year = $('#year').val();
			var month = $('#month').val();
			var customer_id = $('#customer_id').val();
			var mp_id = $("#meteringPoint").val();
			////////////////////////////////////////////
			var form_data = new FormData(); 
			form_data.append('customer_id', customer_id); 
			form_data.append('year', year); 
			form_data.append('month', month); 
			form_data.append('mp_id', mp_id); 
			form_data.append('narration', narration); 
			form_data.append('workspace', workspace); 
			form_data.append('workspace2', workspace2); 
			form_data.append('sfi', sfi); 
			form_data.append('sfe', sfe); 
			form_data.append('wc_sfi', wc_sfi); 
			form_data.append('wc_sfe', wc_sfe); 
			form_data.append('newLabel', newLabel); 
			form_data.append('wheelingCharge', wheelingCharge); 
			form_data.append('wheelingChargeE', wheelingChargeE);
			form_data.append('tlf', tlf); 
			form_data.append('tlfactor', tlfactor); 
			form_data.append('note', note); 

			$.ajax({
	            url: '<?php echo return_url().'classes/formulae-saver.php'; ?>',
	            type: "POST",
	            data: form_data,
	            contentType: false,
	            cache: false,
	            processData:false,
	            success: function(data){
	                
	                var d = JSON.parse(data);
	                if(d.status=="Saved"){
	                	$("#applyStatus").html("<span style='color:green;'>Saved</span>");
	                }else{	                	
	                	$("#applyStatus").html("<span style='color:red;'>"+d.message+"</span>");
	                }
	            }
	        });
		});	

		$('#year').change(function(){
			$('#narration').val("Loading...");
			$('#workspace').val("Loading...");
			$('#workspace2').val("Loading...");
			$('#newLabel').val("Loading...");
			$('#wheelingChargeE').val("Loading...");
			$('#tlf').val("Loading...");
			$('#wheelingCharge').val("Loading...");
			/////////////////////////////////////
			var year = $('#year').val();
			var month = $('#month').val();
			var customer_id = $('#customer_id').val();
			var mp_id = $("#meteringPoint").val();
			var note = $("#note").val();
			////////////////////////////////////////////
			var form_data = new FormData(); 
			form_data.append('customer_id', customer_id); 
			form_data.append('year', year); 
			form_data.append('month', month); 
			form_data.append('mp_id', mp_id); 
			form_data.append('note', note); 

			$.ajax({
	            url: '<?php echo return_url().'classes/formulae-checker.php'; ?>',
	            type: "POST",
	            data: form_data,
	            contentType: false,
	            cache: false,
	            processData:false,
	            success: function(data){
	                
	                var d = JSON.parse(data);
	                $('#narration').val(d.narration); 
	                $('#newLabel').val(d.newLabel);
	                $('#workspace').val(d.workspace); 
	                $('#workspace_').val(d.workspace);
	                $('#workspace2').val(d.workspace2);
	                $('#workspace_2').val(d.workspace2);
	                $('#wheelingCharge').val(d.wheelingCharge);	
	                $('#wheelingCharge_').val(d.wheelingCharge);
	                $('#wheelingChargeE').val(d.wheelingChargeE);	
	                $('#wheelingChargeE_').val(d.wheelingChargeE);	
	                $('#tlf').val(d.tlf);	
	                $('#tlf_').val(d.tlf);	
	            }
	        });
		});	

		var toggler = document.getElementsByClassName("customer-label");
		var i;
		for (i = 0; i < toggler.length; i++) {
		  toggler[i].addEventListener("click", function() {
		    this.parentElement.querySelector(".nested").classList.toggle("active");
		    //this.classList.toggle("caret-down");
		  });
		}

		var textarea = $("#workspace");
	    var len = textarea.val().length;
	    textarea[0].focus();
	    textarea[0].setSelectionRange(len, len);

		var textarea = $("#workspace2");
	    var len = textarea.val().length;
	    textarea[0].focus();
	    textarea[0].setSelectionRange(len, len);

		$.fn.extend({
		  insertAtCaret: function(myValue) {
		    this.each(function() {
		      if (document.selection) {
		        this.focus();
		        var sel = document.selection.createRange();
		        sel.text = myValue;
		        this.focus();
		      } else if (this.selectionStart || this.selectionStart == '0') {
		        var startPos = this.selectionStart;
		        var endPos = this.selectionEnd;
		        var scrollTop = this.scrollTop;
		        this.value = this.value.substring(0, startPos) +
		          myValue + this.value.substring(endPos,this.value.length);
		        this.focus();
		        this.selectionStart = startPos + myValue.length;
		        this.selectionEnd = startPos + myValue.length;
		        this.scrollTop = scrollTop;
		      } else {
		        this.value += myValue;
		        this.focus();
		      }
		    });
		    return this;
		  }
		});

		if($('#side').val() == "Imports"){
			$("#workspace").prop("disabled",false);
			$("#workspace2").prop("disabled",true);
			$("#wheelingCharge").prop("disabled",true);
			$("#wheelingChargeE").prop("disabled",true);
			$("#tlf").prop("disabled",true);
		}else{
			$("#workspace2").prop("disabled",false);
			$("#workspace").prop("disabled",true);
			$("#wheelingCharge").prop("disabled",true);
			$("#wheelingChargeE").prop("disabled",true);
			$("#tlf").prop("disabled",true);
		}
		$('#test').click(function(){
			$("#workspace_").val($('#workspace').val());
			$("#workspace_2").val($('#workspace2').val());
			$("#wheelingCharge_").val($('#wheelingCharge').val());
			$("#wheelingChargeE_").val($('#wheelingChargeE').val());
			$("#tlf_").val($('#tlf').val());
		});

		$('#side').change(function(){
			var textarea = "";
			if($('#side').val() == "Imports"){
				$("#workspace").prop("disabled",false);
				$("#workspace2").prop("disabled",true);
				$("#wheelingCharge").prop("disabled",true);
				$("#wheelingChargeE").prop("disabled",true);
				$("#tlf").prop("disabled",true);
				textarea = $("#workspace");
			}else if($('#side').val() == "Exports"){
				$("#workspace2").prop("disabled",false);
				$("#workspace").prop("disabled",true);
				$("#wheelingCharge").prop("disabled",true);
				$("#wheelingChargeE").prop("disabled",true);
				$("#tlf").prop("disabled",true);
				textarea = $("#workspace");
			}else if($('#side').val() == "wheelingCharge"){
				$("#workspace2").prop("disabled",true);
				$("#workspace").prop("disabled",true);
				$("#wheelingCharge").prop("disabled",false);
				$("#wheelingChargeE").prop("disabled",true);
				$("#tlf").prop("disabled",true);
				textarea = $("#wheelingCharge");
			}else if($('#side').val() == "wheelingChargeE"){
				$("#workspace2").prop("disabled",true);
				$("#workspace").prop("disabled",true);
				$("#wheelingCharge").prop("disabled",true);
				$("#wheelingChargeE").prop("disabled",false);
				$("#tlf").prop("disabled",true);
				textarea = $("#wheelingChargeE");
			}else if($('#side').val() == "tlf"){
				$("#workspace2").prop("disabled",true);
				$("#workspace").prop("disabled",true);
				$("#wheelingCharge").prop("disabled",true);
				$("#wheelingChargeE").prop("disabled",true);
				$("#tlf").prop("disabled",false);
				textarea = $("#tlf");
			}

		    var len = textarea.val().length;
		    textarea[0].focus();
		    textarea[0].setSelectionRange(len, len);
	    	textarea.prop('font-size', '10px');
		});

		$(".operators").click(function(){
			if($('#side').val() == "Imports"){
				if($("#workspace").val()=="([DONOT_DISPLAY])")
					$("#workspace").val("");
				if($(this).val()=="([DONOT_DISPLAY])"){
					$("#workspace").val($(this).val());
				}else{
					$("#workspace").insertAtCaret($(this).val());
				}
				$("#workspace").prop("disabled", false);
				$("#workspace2").prop("disabled", true);
				$("#wheelingCharge").prop("disabled", true);
				$("#tlf").prop("disabled", true);
			}else if($('#side').val() == "Exports"){
				if($("#workspace2").val()=="([DONOT_DISPLAY])")
					$("#workspace2").val("");
				if($(this).val()=="([DONOT_DISPLAY])"){
					$("#workspace2").val($(this).val());
				}else{
					$("#workspace2").insertAtCaret($(this).val());
				}
				$("#workspace2").prop("disabled", false);
				$("#workspace").prop("disabled", true);
				$("#wheelingCharge").prop("disabled", true);
				$("#wheelingChargeE").prop("disabled", false);
				$("#tlf").prop("disabled", true);
			}else if($('#side').val() == "Wheeling Charge"){
				if($("#wheelingCharge").val()=="([DONOT_DISPLAY])")
					$("#wheelingCharge").val("");
				if($(this).val()=="([DONOT_DISPLAY])"){
					$("#wheelingCharge").val($(this).val());
				}else{
					$("#wheelingCharge").insertAtCaret($(this).val());
				}
				$("#workspace2").prop("disabled", true);
				$("#workspace").prop("disabled", true);
				$("#wheelingCharge").prop("disabled", false);
				$("#wheelingChargeE").prop("disabled", true);
				$("#tlf").prop("disabled", true);
			}else if($('#side').val() == "Exports Wheeling Charge"){
				if($("#wheelingChargeE").val()=="([DONOT_DISPLAY])")
					$("#wheelingChargeE").val("");
				if($(this).val()=="([DONOT_DISPLAY])"){
					$("#wheelingChargeE").val($(this).val());
				}else{
					$("#wheelingChargeE").insertAtCaret($(this).val());
				}
				$("#workspace2").prop("disabled", true);
				$("#workspace").prop("disabled", true);
				$("#wheelingCharge").prop("disabled", true);
				$("#wheelingChargeE").prop("disabled", false);
				$("#tlf").prop("disabled", true);
			}else if($('#side').val() == "tlf"){
				if($("#tlf").val()=="([DONOT_DISPLAY])")
					$("#tlf").val("");
				if($(this).val()=="([DONOT_DISPLAY])"){
					$("#tlf").val($(this).val());
				}else{
					$("#tlf").insertAtCaret($(this).val());
				}
				$("#workspace2").prop("disabled", true);
				$("#workspace").prop("disabled", true);
				$("#wheelingCharge").prop("disabled", true);
				$("#wheelingChargeE").prop("disabled", true);
				$("#tlf").prop("disabled", false);
			}
		});

		$(".fields").click(function(){
			//alert("sdfsdf");
			if($('#side').val() == "Imports"){
				if($("#workspace").val()=="([DONOT_DISPLAY])")
					$("#workspace").val("");
				if($(this).val()=="([DONOT_DISPLAY])"){
					$("#workspace").val($(this).val());
				}else{
					$("#workspace").insertAtCaret($(this).attr('data-id'));
				}
				$("#workspace").prop("disabled", false);
				$("#workspace2").prop("disabled", true);
				$("#wheelingCharge").prop("disabled", true);
				$("#wheelingChargeE").prop("disabled", true);
			}else if($('#side').val() == "Exports"){
				if($("#workspace2").val()=="([DONOT_DISPLAY])")
					$("#workspace2").val("");
				if($(this).val()=="([DONOT_DISPLAY])"){
					$("#workspace2").val($(this).val());
				}else{
					$("#workspace2").insertAtCaret($(this).attr('data-id'));
				}
				$("#workspace2").prop("disabled", false);
				$("#workspace").prop("disabled", true);
				$("#wheelingCharge").prop("disabled", true);
				$("#wheelingChargeE").prop("disabled", false);
			}else if($('#side').val() == "wheelingCharge"){
				if($("#wheelingCharge").val()=="([DONOT_DISPLAY])")
					$("#wheelingCharge").val("");
				if($(this).val()=="([DONOT_DISPLAY])"){
					$("#wheelingCharge").val($(this).val());
				}else{
					$("#wheelingCharge").insertAtCaret($(this).attr('data-id'));
				}
				$("#workspace2").prop("disabled", true);
				$("#workspace").prop("disabled", true);
				$("#wheelingCharge").prop("disabled", false);
				$("#wheelingChargeE").prop("disabled", true);
			}else if($('#side').val() == "wheelingChargeE"){
				if($("#wheelingChargeE").val()=="([DONOT_DISPLAY])")
					$("#wheelingChargeE").val("");
				if($(this).val()=="([DONOT_DISPLAY])"){
					$("#wheelingChargeE").val($(this).val());
				}else{
					$("#wheelingChargeE").insertAtCaret($(this).attr('data-id'));
				}
				$("#workspace2").prop("disabled", true);
				$("#workspace").prop("disabled", true);
				$("#wheelingCharge").prop("disabled", true);
				$("#wheelingChargeE").prop("disabled", false);
			}else if($('#side').val() == "tlf"){
				if($("#tlf").val()=="([DONOT_DISPLAY])")
					$("#tlf").val("");
				if($(this).val()=="([DONOT_DISPLAY])"){
					$("#tlf").val($(this).val());
				}else{
					$("#tlf").insertAtCaret($(this).attr('data-id'));
				}
				$("#workspace2").prop("disabled", true);
				$("#workspace").prop("disabled", true);
				$("#wheelingCharge").prop("disabled", true);
				$("#wheelingChargeE").prop("disabled", true);
				$("#tlf").prop("disabled", false);
			}
		});

		$('#clear').click(function(){
			if(confirm("Do you want to Clear Workspace?")){
				$("#workspace").val("");
				$("#workspace2").val("");
				$("#wheelingCharge").val("");
				$("#wheelingChargeE").val("");
				$("#tlf").val("");
			}
		});
	</script>
	<?php

	}//ifelse
	}//locked
	}


	public function fx(){


		$class = ($this->set_class)?$this->set_class:portion(1);
		$method = ($this->set_method)?$this->set_method:portion(2);
		$id = ($this->set_customer)?$this->set_customer:portion(3);
		$year = ($this->set_year)?$this->set_year: portion(4);
		$month = ($this->set_month)?$this->set_month: portion(5);

		$note = portion(6);

		$year_uploaded = $year;
		$month_uploaded = $month;

		if(empty($year) || empty($month) || empty($id)){
			Feedback::error("Please select Customer, Year and Month");
			$units = $this->customerAndSchedule($id, $year, $month);
			$this->energyLinks($id, $year, $month);
			$units = 0.001;
			echo '<div class="clearfix"></div>';
		}else{
			//Feedback::error("No readings found");
			$units = $this->customerAndSchedule($id, $year, $month);
			$this->energyLinks($id, $year, $month);
			$units = 0.001;
			echo '<div class="clearfix"></div>';
			echo '</br>';
		}

		$db = new Db();


		echo '<div id="printMe">';

		if($id == 2017 || $id == 2029){

			$q1 = array(1,2,3);
			$q2 = array(4,5,6);
			$q3 = array(7,8,9);
			$q4 = array(10,11,12);

			$use = "";

			if(in_array($month, $q1)){
				$use = "q1";
				$factor_month = $$use[0];
			}elseif(in_array($month, $q2)){
				$use = "q2";
				$factor_month = $$use[0];
			}elseif(in_array($month, $q3)){
				$use = "q3";
				$factor_month = $$use[0];
			}elseif(in_array($month, $q4)){
				$use = "q4";
				$factor_month = $$use[0];
			}


			$select = $db->select("SELECT * FROM generators ORDER BY ge_name ASC");

			echo '<table id="" border="1" cellpadding="2" cellspacing="0" style="font-family:arial;font-size:14px;">';
			echo '<thead>';
			echo '<tr align="center" valign="bottom" style="">';			
			echo '<th style="width:30px">No.</th>';
			echo '<th style="">Generator</th>';

			foreach($$use as $m){
				echo '<th style="min-width:100px;"><b>'.month_name($m).'</b></th>';
			}
			echo '</tr>';
			$i=0;
			foreach($select[0] as $row){
				extract($row);
				echo  '<tr>';
				echo '<td>'.(++$i).'.</td>';
				echo '<td>'.($ge_name).'</td>';
				foreach($$use as $m){
					$v = $this->get_bill_price2($year, $m, $ge_id);
					echo '<td>'.$v['formulae'].'</td>';
				}
				echo '</tr>';
			}

			echo '</table>';
			echo '<br/><br/>';



			echo '</div>';

		}else{

			$select = $db->select("SELECT * FROM metering_point WHERE mp_customer_id = '$id' ORDER BY mp_location ASC");

			echo '<table style="width:100%;font-size:12px;">';
			echo '<thead>';
			echo '<tr>';
			echo '<th colspan="3">'.$this->rgf("customer", $id, "customer_id", "customer_short_name").' FORMULAE</th>';
			echo '</tr>';

			echo '<tr>';
			echo '<th style="text-align:left;">No.</th>';
			echo '<th style="text-align:left;">Metering Point</th>';
			echo '<th style="text-align:left;">Formulae</th>';
			echo '</tr>';

			echo '</thead>';
			echo '<tbody>';
			$no=1;
			foreach($select[0] as $row){
				extract($row);

				echo '<tr valign="top">';
				echo '<td style="border-top:2px solid #000;padding:2px;">'.($no++).'.</td>';
				echo '<td style="border-top:2px solid #000;padding:2px;">'.$mp_location.'</td>';
				echo '<td style="border-top:2px solid #000;padding:2px;">';

				$this->formulaDisplay2($id, $mp_id, $year, $month);

				echo '</td style="border-bottom:2px solid #000;">';
				echo '<td>'.'</td>';
				echo '</tr>';
			}
			echo '</tbody>';
			echo '</table>';

			echo '</div>';
		}

	}

	public function fxDetails(){
		
		$class = ($this->set_class)?$this->set_class:portion(1);
		$method = ($this->set_method)?$this->set_method:portion(2);
		$id = ($this->set_customer)?$this->set_customer:portion(3);
		$year = ($this->set_year)?$this->set_year: portion(4);
		$month = ($this->set_month)?$this->set_month: portion(5);

		$note = portion(6);

		$year_uploaded = $year;
		$month_uploaded = $month;

		if(empty($year) || empty($month) || empty($id)){
			Feedback::error("Please select Customer, Year and Month");
			$units = $this->customerAndSchedule($id, $year, $month);
			$this->energyLinks($id, $year, $month);
			$units = 0.001;
			echo '<div class="clearfix"></div>';
		}else{
			//Feedback::error("No readings found");
			$units = $this->customerAndSchedule($id, $year, $month);
			$this->energyLinks($id, $year, $month);
			$units = 0.001;
			echo '<div class="clearfix"></div>';
			echo '</br>';
		}

		$db = new Db();


		echo '<div id="printMe">';

				if($id == 2017 || $id == 2029){

			$q1 = array(1,2,3);
			$q2 = array(4,5,6);
			$q3 = array(7,8,9);
			$q4 = array(10,11,12);

			$use = "";

			if(in_array($month, $q1)){
				$use = "q1";
				$factor_month = $$use[0];
			}elseif(in_array($month, $q2)){
				$use = "q2";
				$factor_month = $$use[0];
			}elseif(in_array($month, $q3)){
				$use = "q3";
				$factor_month = $$use[0];
			}elseif(in_array($month, $q4)){
				$use = "q4";
				$factor_month = $$use[0];
			}


			$select = $db->select("SELECT * FROM generators ORDER BY ge_name ASC");

			echo '<table id="" border="1" cellpadding="2" cellspacing="0" style="font-family:arial;font-size:14px;">';
			echo '<thead>';
			echo '<tr align="center" valign="bottom" style="">';			
			echo '<th style="width:30px">No.</th>';
			echo '<th style="">Generator</th>';

			foreach($$use as $m){
				echo '<th style="min-width:100px;"><b>'.month_name($m).'</b></th>';
			}
			echo '</tr>';
			$i=0;
			foreach($select[0] as $row){
				extract($row);
				echo  '<tr>';
				echo '<td>'.(++$i).'.</td>';
				echo '<td>'.($ge_name).'</td>';
				foreach($$use as $m){
					echo '<td>';
					$v = $this->get_bill_price2($year, $m, $ge_id);
					echo '<b>'.$v['formulae'].'</b><br/>';
					$this->get_bill_price($ge_id, $year, $m, $v['formulae'], 1);
					echo '</td>';
				}
				echo '</tr>';
			}

			echo '</table>';
			echo '<br/><br/>';



			echo '</div>';

		}else{
			$select = $db->select("SELECT * FROM metering_point WHERE mp_customer_id = '$id' ORDER BY mp_location ASC");

			echo '<table style="width:100%;font-size:12px;">';
			echo '<thead>';
			echo '<tr>';
			echo '<th colspan="3">'.$this->rgf("customer", $id, "customer_id", "customer_short_name").' FORMULAE '.$year.' '.month_name($month).'</th>';		
			echo '</tr>';

			// echo '<tr>';
			// echo '<th>No.</th>';
			// echo '<th>Metering Point</th>';
			// //echo '<th></th>';
			// echo '</tr>';

			echo '</thead>';
			echo '<tbody>';
			$no=1;
			foreach($select[0] as $row){
				extract($row);

				echo '<tr valign="top">';
				echo '<td style="border-top:2px solid #000;padding:5px;">'.($no++).'.</td>';
				echo '<td style="border-top:2px solid #000;padding:5px;">';
				$date = strtotime($year.'-'.$month.'-15');
				$r = rate_calculate($id, $date, $mp_id);
				$s = ($r[18])?" (S)":"";//checking swapping point
				$aka = ($r[21])?" <span style='color:red;'><i> aka ".$r[21]."</i></span>":"";
				echo '<b>'.$mp_location.$s.$aka.'</b><br/>';
				echo '<div>';
				//$this->formulaDisplayFx($id, $mp_id, $year, $month);
				echo '</div>';
				echo '<div style="font-size:10px;padding:2px; padding-left:5px; float:left;">';
				echo '<b>(CR)<br/>Current&nbsp;Readings:</b><br/>';
				echo 'R1 = '.number_format($r[19][0]/1000).'<br/>';
				echo 'R2 = '.number_format($r[19][1]/1000).'<br/>';
				echo 'R3 = '.number_format($r[19][2]/1000).'<br/>';
				echo 'R4 = '.number_format($r[19][3]/1000).'<br/>';
				echo 'R5 = '.number_format($r[19][4]/1000).'<br/>';
				echo 'R6 = '.number_format($r[19][5]/1000).'';
				echo '</div>';
				echo '<div style="font-size:10px;padding:2px; padding-left:5px; float:left;">';
				echo '<b>(PR)<br/>Pervious Readings:</b><br/>';
				echo 'R1 = '.number_format($r[20][0]/1000).'<br/>';
				echo 'R2 = '.number_format($r[20][1]/1000).'<br/>';
				echo 'R3 = '.number_format($r[20][2]/1000).'<br/>';
				echo 'R4 = '.number_format($r[20][3]/1000).'<br/>';
				echo 'R5 = '.number_format($r[20][4]/1000).'<br/>';
				echo 'R6 = '.number_format($r[20][5]/1000).'';
				echo '</div>';
				echo '<div style="font-size:10px;padding:2px; padding-left:5px; float:left;">';
				echo '<b>(AR=CR-PR)<br/>Advance</b><br/>';
				echo 'AR1 = '.number_format(($r[19][0]-$r[20][0])/1000).'<br/>';
				echo 'AR2 = '.number_format(($r[19][1]-$r[20][1])/1000).'<br/>';
				echo 'AR3 = '.number_format(($r[19][2]-$r[20][2])/1000).'<br/>';
				echo 'AR4 = '.number_format(($r[19][3]-$r[20][3])/1000).'<br/>';
				echo 'AR5 = '.number_format(($r[19][4]-$r[20][4])/1000).'<br/>';
				echo 'AR6 = '.number_format(($r[19][5]-$r[20][5])/1000).'';
				echo '</div>';
				echo '<div style="width:60%;float:left;">';
				$this->formulaDisplay4($id, $mp_id, $year, $month);
				echo '</div>';
				//echo 'R1='.number_format($r[0]/1000).'<br/>';
				echo '</td>';
				// echo '<td style="border-top:2px solid #000;padding:5px;">';

				// //$this->formulaDisplay3($id, $mp_id, $year, $month);

				// echo '</td style="border-bottom:2px solid #000;">';
				//echo '<td>'.'</td>';
				echo '</tr>';
			}
			echo '</tbody>';
			echo '</table>';

			echo '</div>';
		}
	}
	
}



