<?php

Class Reports extends BeforeAndAfter{
	public $page = "REPORTS";
	
	public function __construct(){
		$access = new AccessRights();
		//$access->pageAccess(user_id(), $this->page, 'V');
	}
	
	public function getLinks(){
		$page = "REPORTS";
		$links = array(
			array(
				"link_name"=>"Serial Number Finder", 
				"link_address"=>"reports/serial-number-finder",
				"link_icon"=>"fa-search",
				"link_page"=>$page,
				"link_right"=>"V",
			),
			array(
				"link_name"=>"Summary List", 
				"link_address"=>"reports/summary-list",
				"link_icon"=>"fa-list",
				"link_page"=>$page,
				"link_right"=>"V",
			)
		);
		
		return $links;
	}

	public function typeFinder($id, $search){
		if($id == "c"){
			return "LIKE '%$search%'";
		}else if($id == "e"){
			return "LIKE '%$search'";
		}else if($id == "s"){
			return "LIKE '$search%'";
		}else if($id == "p"){
			return "LIKE '$search'";
		}else{
			return $id." '$search'";
		}
	}

	public function serialNumberFinderAction(){
		$db = new Db();

		$values = array();

			$tables = array(

				//table->tableid, date, addedby, link

				"vehicle_request"=>array(
					"vr_id", 
					"vr_number", 
					"vr_date_added", 
					"vr_requestor_id", 
					"approve-vehicle-request/vehicle-request-details/"
				),

				"pool_fuel_request"=>array(
					"pfr_id",
					"pfr_number", 
					"pfr_date_added",
					"pfr_added_by",
					"pool-fuel-request/make-pool-vehicle-fuel-request/"
				),

				"pool_fuel_acc_req"=>array(
					"pfar_id",
					"pfar_number", 
					"pfar_date_added",
					"pfar_requestor_id",
					"pool-fuel-accountability/make-pool-fuel-accountability-request/"
				),

				"vehicle_return"=>array(
					"vehicle_return_id",
					"vehicle_return_number", 
					"vehicle_return_date_added",
					"vehicle_return_requestor_id",
					"vehicle-return/vehicle-return-details/"
				),
			);


		if(isset($_POST['submit'])){
		
			$serial_number = $_POST['serial_number'];
			$type = $_POST['type'];

			if(empty($serial_number)){
				$errors[] = "Enter Serial Number";
			}
			if(empty($type)){
				$errors[] = "Select Type";
			}
			
			$results = array();

			if(empty($errors)){	
				$toSearch = $this->typeFinder($type, $serial_number);
				$total = array();
				foreach($tables as $key=>$values){				

					$id = $values[0];
					$number = $values[1];
					$date = $values[2];
					$added_by = $values[3];
					$link = $values[4];

					$query = "SELECT $id as id2, $number as number2, $date as date2, $added_by as added_by2 FROM $key WHERE $number $toSearch;";

					$select = $db->select($query);
					
					if($db->num_rows()){
						$total[$key] = $db->num_rows();
						foreach($select[0] as $sel){
							extract($sel);
							$results[] = array(
								'id'=>$id2, 
								'number'=>$number2, 
								'date'=>$date2, 
								'added_by'=>$added_by2, 
								'link'=>$link
							);
							
						}
					}
				}
			}else{
				FeedBack::errors($errors);
			}
		}
		?>
		<div class="row">
			<div class="col-lg-12">
			<div class="panel panel-default">
				<div class="panel-body">
					<div class="row">
						<form role="form" action="" method="post">
							<div class="col-lg-12">
								<label>Enter Serial No.:<span class="must">&nbsp;</span></label>
							</div>
							<div class="col-lg-3">
								<div class="form-group">									
									<div class="form-line">
										<input class="form-control" type="text" name="serial_number" placeholder="Enter number" value="<?php echo @$serial_number; ?>">
									</div>
								</div>								
							</div>
							<div class="col-lg-9">
								<div class="form-group">
									<div class="demo-radio-button">								

										<input name="type" type="radio" id="radio_8" value="c" class="radio-col-red alty"   <?php echo $x = ($type == "c")? 'checked="checked"': 'checked="checked"';  ?>>
										<label for="radio_8" >Contains</label>

										<input name="type" type="radio" id="radio_7" value="p" class="radio-col-red alty"   <?php echo $x = ($type == "p")? 'checked="checked"': ''; ?>>
										<label for="radio_7">Perfect Match</label>
										
										

										<input name="type" type="radio" id="radio_9" value="s" class="radio-col-red alty"   <?php echo $x = ($type == "s")? 'checked="checked"': ""; ?>>
										<label for="radio_9" >Start With</label>

										<input name="type" type="radio" id="radio_10" value="e" class="radio-col-red alty"  <?php echo $x = ($type == "e")? 'checked="checked"': ""; ?>>
										<label for="radio_10" >Ends</label>	

									</div>
								</div>
							</div>
							<div class="clearfix"></div>
							<div class="col-lg-12">
								<button type="submit" name="submit" class="btn btn-primary"><i class="fa fa-fw fa-search"></i> Search</button>
							</div>
						</form>
					</div>
				</div>
			</div>
		</div>
	</div>
	<div class="row">
		<div class="">			
			<div class="panel1 panel-default">
				<div class="panel-body">
					<?php
					if(isset($_POST['submit'])){
						$total_added= array_sum($total);
						$details = array();
						foreach($total as $key => $value){
							$details[] = "$key = $value";
						}
						$details = "";// (".implode(' , ', $details).' ) ';
						if($total_added==1){
							echo 'Your search returned: <b>'.$total_added.' Result '.$details.'</b>';
						}else{
							echo 'Your search returned: <b>'.$total_added.' Results '.$details.'</b>';
						}

						if($total_added >= 1){
							echo '<br/><br/>';
							echo '<table border="1" id="table">';
							echo '<thead>';
							echo '<tr>';
							echo '<th width="30px">No.</th>';
							echo '<th>Serial Number</th>';
							echo '<th>Requestor / Submitted by</th>';
							echo '<th>Date</th>';
							echo '</tr>';
							echo '</thead>';
							$no = 1;

							////////////////////////////REPORT STEP 1//////////////////
							$db_values = array();
							$db_values[] = array(
								"No",
								"Serial Number",
								"Requestor / Submitted by",
								"Date"							
							);
							//////////////////////////////////////////////////////////////

							echo '<tbody>';
							foreach($results as $row){
								extract($row);
								echo '<tr>';
								echo '<td>'.($no++).'</td>';
								echo '<td><a target="_blank" href="'.return_url().$link.$id.'">'.$number.'</a></td>';	
								echo '<td>'.$this->full_name($added_by).'</td>';							
								echo '<td>'.Feedback::date_fm($date).'</td>';
								echo '</tr>';

								/////////////////////////REPORT STEP 2//////////////////////	
								$db_values[] = array(
									($no-1),
									($number),
									($this->full_name($added_by)),
									Feedback::date_fm($date)
														
								); 
								
								///////////////////////////////////////////////////////////
							}
							echo '</tbody>';
							echo '</table>';

							///////////////////////REPORT STEP 3//////////////////////////
							$t = new TableCreator();
							$heading = "SERIAL NUMBER LIST"; //CHANGE
							$t->open($this->full_name(user_id()), $heading);
							$t->thd($db_values);
							$t->close();
							$t->results();

							$e = new Exporter();
							echo $e->getDisplay($heading, $t->results());
							///////////////////////////////////////////////////////

							

						}	
					}
					?>
				</div>
			</div>
		</div>
	</div>
	<?php
	}


	public function summaryListAction(){
		$db = new Db();
		$requiredReports = array(

			"System Users"=>array(

				"table"=>"sysuser",

				"columns"=>array(				

					"UserName" => array(						
						"db_col"=>"user_name", 
						"data_type"=>"text", 
						"default"=>"0",
						//"match_to"=>array("vehicle", "vehicle_id", "vehicle_reg_no"),
						//"method_caller"=>array("FuelCards", "balanceCalculator"),
						//"total"=>array("FuelCard", "balanceCalculator"),
					),				

					"Password" => array(						
						"db_col"=>"user_password", 
						"data_type"=>"text", 
						"default"=>"0",
						//"match_to"=>array("vehicle", "vehicle_id", "vehicle_reg_no"),
						//"method_caller"=>array("FuelCards", "balanceCalculator"),
						//"total"=>array("FuelCard", "balanceCalculator"),
					),
					
					"Surname" => array(						
						"db_col"=>"user_surname", 
						"data_type"=>"text", 
						"default"=>"1",
						//"match_to"=>array("vehicle", "vehicle_id", "vehicle_reg_no"),
						//"method_caller"=>array("FuelCards", "balanceCalculator"),
						//"total"=>array("FuelCard", "balanceCalculator"),
					),
					
					"Othername(s)" => array(						
						"db_col"=>"user_othername", 
						"data_type"=>"text", 
						"default"=>"1",
						//"match_to"=>array("vehicle", "vehicle_id", "vehicle_reg_no"),
						//"method_caller"=>array("FuelCards", "balanceCalculator"),
						//"total"=>array("FuelCard", "balanceCalculator"),
					),
					
					"Check No." => array(						
						"db_col"=>"check_number", 
						"data_type"=>"text", 
						"default"=>"1",
						//"match_to"=>array("vehicle", "vehicle_id", "vehicle_reg_no"),
						//"method_caller"=>array("FuelCards", "balanceCalculator"),
						//"total"=>array("FuelCard", "balanceCalculator"),
					),

					/*
					"Gender" => array(						
						"db_col"=>"user_gender", 
						"data_type"=>"text", 
						"default"=>"0",
						//"match_to"=>array("users", "vehicle_id", "vehicle_reg_no"),
						"method_caller"=>array("Users", "gender"),
						//"total"=>array("FuelCard", "balanceCalculator"),
					),
					*/
					
					"Phone No." => array(						
						"db_col"=>"user_telephone", 
						"data_type"=>"text", 
						"default"=>"0",
						//"match_to"=>array("vehicle", "vehicle_id", "vehicle_reg_no"),
						//"method_caller"=>array("FuelCards", "balanceCalculator"),
						//"total"=>array("FuelCard", "balanceCalculator"),
					),
					
					"Email Address" => array(						
						"db_col"=>"user_email", 
						"data_type"=>"text", 
						"default"=>"1",
						//"match_to"=>array("vehicle", "vehicle_id", "vehicle_reg_no"),
						//"method_caller"=>array("FuelCards", "balanceCalculator"),
						//"total"=>array("FuelCard", "balanceCalculator"),
					),

					"Designation" => array(						
						"db_col"=>"user_designation", 
						"data_type"=>"text", 
						"default"=>"1",
						"match_to"=>array("designation", "designation_id", "designation_name"),
						//"method_caller"=>array("FuelCards", "balanceCalculator"),
						//"total"=>array("FuelCard", "balanceCalculator"),
					),					
					
					"Office" => array(						
						"db_col"=>"user_branch_id", 
						"data_type"=>"text", 
						"default"=>"1",
						"match_to"=>array("branch", "branch_id", "branch_name"),
						//"method_caller"=>array("FuelCards", "balanceCalculator"),
						//"total"=>array("FuelCard", "balanceCalculator"),
					),					
					
					"Department" => array(						
						"db_col"=>"user_department_id", 
						"data_type"=>"text", 
						"default"=>"1",
						"match_to"=>array("department", "dept_id", "dept_name"),
						//"method_caller"=>array("FuelCards", "balanceCalculator"),
						//"total"=>array("FuelCard", "balanceCalculator"),
					),				
					
					"Territory" => array(						
						"db_col"=>"user_territory_id", 
						"data_type"=>"text", 
						"default"=>"1",
						"match_to"=>array("territory", "territory_id", "territory_name"),
						//"method_caller"=>array("FuelCards", "balanceCalculator"),
						//"total"=>array("FuelCard", "balanceCalculator"),
					),			
					
					"Section" => array(						
						"db_col"=>"user_section_id", 
						"data_type"=>"text", 
						"default"=>"1",
						"match_to"=>array("section", "section_id", "section_name"),
						//"method_caller"=>array("FuelCards", "balanceCalculator"),
						//"total"=>array("FuelCard", "balanceCalculator"),
					),		
					
					"Date Created" => array(						
						"db_col"=>"user_date_added", 
						"data_type"=>"date", 
						"default"=>"0",
						//"match_to"=>array("section", "section_id", "section_name"),
						//"method_caller"=>array("FuelCards", "balanceCalculator"),
						//"total"=>array("FuelCard", "balanceCalculator"),
					),		
					
					"Created By" => array(						
						"db_col"=>"user_added_by", 
						"data_type"=>"text", 
						"default"=>"0",
						"method_caller"=>array("VehicleRequest", "full_name"),
						//"method_caller"=>array("FuelCards", "balanceCalculator"),
						//"total"=>array("FuelCard", "balanceCalculator"),
					),	
					
					"Last Logged In" => array(						
						"db_col"=>"user_last_logged_in", 
						"data_type"=>"date", 
						"default"=>"0",
						//"match_to"=>array("sysuser", "user_id", "user_surname"),
						//"method_caller"=>array("FuelCards", "balanceCalculator"),
						//"total"=>array("FuelCard", "balanceCalculator"),
					),
						
					
					"Password Last Changed" => array(						
						"db_col"=>"user_last_changed", 
						"data_type"=>"date", 
						"default"=>"0",
						//"match_to"=>array("sysuser", "user_id", "user_surname"),
						//"method_caller"=>array("FuelCards", "balanceCalculator"),
						//"total"=>array("FuelCard", "balanceCalculator"),
					),
					
					
				),
				
			),

			"Departments"=>array(
				"table"=>"department",
				"columns"=>array(
					"Date Created" => array(						
						"db_col"=>"dept_date_added", 
						"data_type"=>"date", 
						"default"=>"1",
					),					
					"Created by" => array(						
						"db_col"=>"dept_added_by", 
						"data_type"=>"text", 
						"default"=>"1",
						"method_caller"=>array("FuelCards","full_name"),
					),					
					"Department name" => array(						
						"db_col"=>"dept_name", 
						"data_type"=>"text", 
						"default"=>"1",
					),
				),
			),

			"Audit Trail"=>array(
				"table"=>"trail_of_users",
				"columns"=>array(
					"Date" => array(						
						"db_col"=>"trail_date", 
						"data_type"=>"date", 
						"default"=>"1",
					),					
					"User" => array(						
						"db_col"=>"trail_username", 
						"data_type"=>"text", 
						"default"=>"1",
						"method_caller"=>array("FuelCards","full_name"),
					),					
					"IP Address" => array(						
						"db_col"=>"trail_ip", 
						"data_type"=>"text", 
						"default"=>"1",
					),
					"Action" => array(						
						"db_col"=>"trail_action", 
						"data_type"=>"text", 
						"default"=>"1",
					),
					"Page" => array(						
						"db_col"=>"trail_page", 
						"data_type"=>"text", 
						"default"=>"1",
					),
					"Browser" => array(						
						"db_col"=>"trail_browser", 
						"data_type"=>"text", 
						"default"=>"1",
					),
					"Description" => array(						
						"db_col"=>"trail_description", 
						"data_type"=>"text", 
						"default"=>"1",
					),
					"SQL Query" => array(						
						"db_col"=>"trail_sql", 
						"data_type"=>"text", 
						"default"=>"0",
					),//trail_sql

				),
			),
			
			"Designation"=>array(
				"table"=>"designation",
				"columns"=>array(
					"Name" => array(						
						"db_col"=>"designation_name", 
						"data_type"=>"text", 
						"default"=>"1",
					),	
					
					"Total Staff" => array(						
						"db_col"=>"designation_id", 
						"data_type"=>"number", 
						"default"=>"1",
						"sum"=>array("sysuser", "user_designatio"),
					),

					"Date added" => array(						
						"db_col"=>"designation_date_added", 
						"data_type"=>"date", 
						"default"=>"1",
					),
					"Added by" => array(						
						"db_col"=>"designation_added_by", 
						"data_type"=>"text", 
						"default"=>"1",
						"method_caller"=>array("FuelCards","full_name"),
					),
	
				),
			),

			

		);

		$symbols = array(
			"text"=>array(
				"Contains"=>"c",
				"Ends Withs"=>"e",
				"Starts With"=>"s"
			),
			"number"=>array(
				"Between"=>"between",
				"Greather or Equal"=>">=",
				"Less or Equal"=>"<=",
				"Greater Than"=>">",
				"Less Than"=>"<",
				"Equals"=>"=",
				"No Equal"=>"!=",
				
			),
			"date"=>array(
				"Between"=>"between",
				"Is equal & After"=>">=",
				"Is equal & before"=>"<=",
				"Equals"=>"=",
				"After"=>">",				
				"Before"=>"<",				
				"No Equal"=>"!=",
			),
			"float"=>array(
				"Equals"=>"=",
				"Greater Than"=>">",
				"Greather or Equal"=>">=",
				"Less Than"=>"<",
				"Less or Equal"=>"<=",
				"No Equal"=>"!=",
			)
		);

		$order = array('', 'ASC','DESC');
		$report_format = array('EXCEL','CSV', 'EXCEL');

		if(isset($_POST['generate_report'])){
			$error = array();
			$to_return = $_POST['to_return'];
			$table = $_POST['table'];
			$tab = $_POST['tab'];
			$dat_typs = $_POST['dat_typ'];
			$cols = $_POST['col'];
			$bets = $_POST['bet'];
			$all_cols = implode(' , ', $cols);

			$ches = $_POST['che'];
			$chs = $_POST['ch'];
			$checks = $_POST['check'];

			$cols_to_use = array();

			for($m=0; $m <count($ches); $m++){
				if(in_array($ches[$m], $checks)){
					$cols_to_use[] = $ches[$m];
				}
			}

			$newChecks = array();
			for($t=0; $t<count($ches); $t++){
				if(in_array($ches[$t], $checks)){
					$newChecks[] = $chs[$t];
				}
			}

			$all_checks = implode(' , ', $newChecks);

			$syms = $_POST['sym'];

			$cits = $_POST['cit'];

			$where = array();
			for($m=0; $m <count($cits); $m++){				
				if($cits[$m] !== ""){ 
					if($cits[$m] > $bets[$m] && $bets[$m] != ""){
						$error[] = $ches[$m]." first value should be lessthan than the second value.";
					}
					if($syms[$m]=="between"){
						if($bets[$m] == ""){
							$error[] = $ches[$m]." is missing a second value for between";
						}
						if($dat_typs[$m]=="date"){
							$where[] = $cols[$m].' BETWEEN '.strtotime($cits[$m]).' AND '.strtotime($bets[$m]).'';
						}else{
							$where[] = $cols[$m].' BETWEEN '.$cits[$m].' AND '.$bets[$m].'';
						}
					}else if($dat_typs[$m]=="date"){
						$where[] = $cols[$m].' '.$this->typeFinder($syms[$m], strtotime($cits[$m]));
					}else{
						$where[] = $cols[$m].' '.$this->typeFinder($syms[$m], $cits[$m]);
					}
				}else if($cits[$m] == "" && $bets[$m] != ""){
					$error[] = $ches[$m]." is missing a second value for between";
				}
			}

			if(!empty($where)){
				$where = ' WHERE '.implode(' AND ', $where);
			}else{
				$where = "";
			}			

			$orders = $_POST['order'];

			$all_orders = array();
			for($i=0; $i<count($cols); $i++){
				if(!empty($orders[$i])){
					$all_orders[] = $cols[$i].' '.$orders[$i];
				}
			}

			if(!empty($all_orders)){
				$all_orders = " ORDER BY ".implode(' , ', $all_orders);
			}else{
				$all_orders = "";
			}
			if(!empty($to_return)){
				$top = " TOP $to_return ";
			}else{
				$top = "";
			}
			//echo '<br/>';
			$query = "SELECT $top $all_checks FROM $tab $where $all_orders";

			$select = $db->select($query);

			//echo '>>>> '.$db->num_rows().' <<<< ';

			if(!empty($error)){
				Feedback::errors($error);
			}
			
		}
		echo '
			<style type="text/css">
				input, select, option, button{
					padding:0px !important;
					height: 24px !important;
				}
				#ac input, #ac select, #ac option{
					background-color: #f9f7f7 !important;
				}
				input{ padding:0px 5px !important; }
				input[type=date]{ padding:0px !important; }
			</style>
		';
		echo '<form action="" method="post">';
		echo '<div class="row">';
		echo '<div class="col-lg-12" style="background-color: white; border: 1px solid black;">';
		echo '<b>Choose Report:</b>';
		echo '<div class="clearfix"></div> ';
		echo '<div class="">';
		echo '<ol style="padding:0;margin-left:15px;">';
		$i=0;
		$ii=0;
		echo "<div>";
		foreach($requiredReports as $key => $values){
			if($ii%6==0){
				echo '</div><div class="col-lg-3">';
			}
				$k = str_replace(' ', '_', $key);
				if($k == portion(3)){
					echo '<li><a href="'.return_url().'reports/summary-list/'.$k.'" style="color:red;font-weight:bold;">'.$key.'</a></li>';
				}else{
					echo '<li><a href="'.return_url().'reports/summary-list/'.$k.'">'.$key.'</a></li>';
				}

			if($ii%5==0){
				//echo '</div>';
			}
			
			$ii++;

		}
		echo '</ol>';
		echo '<div class="clearfix"></div>';
		echo '<br/>';
		echo '</div>';

		echo '</div>';		
		echo '<div class="col-lg-12">';


		echo '<div class="clearfix"></div> ';
		$tableName = $sent = str_replace('_', ' ', portion(3));

		if($sent != ""){
		foreach($requiredReports[$sent] as $key => $values){
			//echo '<div>';

			if(is_array($values)){
				if($key == "columns"){
					$number_of_columns = count($values);

					
					echo '<div class="clearfix"></div>';
					echo '</div>';
					echo '<table>';
					echo '<div class="clearfix"></div> ';
					$c = 1;

					$fields = array('gS'=>'Show', 'gC'=>'Column', 'gO'=>'Order', 'gW'=>'Which', 'gV'=>'Value', 'gA'=>'And', 'gB'=>'Between');

					foreach($fields as $field=>$fiel_value){
						${$field} = array();
					}

					foreach($values as $val=>$va){	
						$data_type = $va['data_type'];
						$default = $va['default'];
						$match_to = $va['match_to'];
						
						$c++;

						$method_caller = $va['method_caller'];
						$total = $va['total'];

						if(!empty($method_caller) || !empty($total) || !empty($match_to)){
							
							$gS[$c-2] .= '<input type="hidden" value="'.$val.'" name="che[]"/>';
							$gS[$c-2] .='<input type="hidden" value="'.$va["db_col"].'" name="ch[]"/>';

							//if(!empty($match_to) ){
							//if(0 ){
							if(1){
								$gS[$c-2] .= '<div style="width:100%; text-align:center;"><input style="margin:auto;" type="checkbox" value="'.$val.'" name="check[]" ';
								if(isset($_POST['generate_report'])){
									if(in_array($val, $checks)){
									 	$gS[$c-2] .= ' checked="checked" ';
									}else{
										$gS[$c-2] .=  '';
									}
								}else{
									if($default){
									 	$gS[$c-2] .=  ' checked="checked" ';
									}else{
										$gS[$c-2] .=  '';
									}
								}
								$gS[$c-2] .=  ' id="md_checkbox_'.$c.'" class="filled-in chk-col-red" />';
		                       $gS[$c-2] .=  '<label for="md_checkbox_'.$c.'"></label></div>';
		                    	
		                    }else{
		                    	$gS[$c-2] .= ' &nbsp; &nbsp; &nbsp; &nbsp; ';
		                    	$gS[$c-2] .= '<input type="hidden" value="'.$va["db_col"].'" name="check[]" />';
		                	}
												
							$gC[$c-2] .=  '<select name="co[]" style="max-width:130px !important; background-color: #fbeeee !important;">';			
							$gC[$c-2] .= '<option value="'.$va["db_col"].'">'.$val.'</option>';								
							$gC[$c-2] .= '</select>';
							
							////////////////////////////////////////////////////////
	                    	$gV[$c-2] .='<input type="hidden" name="cit[]" value="">';		
							$gV[$c-2] .= '<input type="hidden" name="dat_typ[]" value=""/>';	
							$gW[$c-2] .= '<input type="hidden" name="sym[]" value=""/>';
							$gO[$c-2] .= '<input type="hidden" name="order[]" value="" />';
							$gB[$c-2] .= '<input type="hidden" name="bet[]" value="" />';	
							$gC[$c-2] .=   '<input type="hidden" name="col[]" style="max-width:130px;" value="'.$va["db_col"].'" />';	
							//$gC[$c-2] .=   '<input type="text" style="max-width:130px;" value="tttt'.$val.'" />';					
							///////////////////////////////////////////////////////////

						}else{
							
							$gS[$c-2] .='<input type="hidden" value="'.$val.'" name="che[]"/>';
							$gS[$c-2] .= '<input type="hidden" value="'.$va["db_col"].'" name="ch[]"/>';

							$gS[$c-2] .= '<div style="width:100%; text-align:center;"><input style="margin:auto;" type="checkbox" value="'.$val.'" name="check[]" ';
							if(isset($_POST['generate_report'])){
								if(in_array($val, $checks)){
								 	$gS[$c-2] .= ' checked="checked" ';
								}else{
									$gS[$c-2] .= '';
								}
							}else{
								if($default){
								 	$gS[$c-2] .= ' checked="checked" ';
								}else{
									$gS[$c-2] .= '';
								}
							}
							$gS[$c-2] .= ' id="md_checkbox_'.$c.'" class="filled-in chk-col-red" />';
	                        $gS[$c-2] .= '<label for="md_checkbox_'.$c.'"></label></div>';
	                        
	                    													
							$gC[$c-2] .=  '<select class="form-control" name="col[]" style="max-width:130px">';			
							$gC[$c-2] .=  '<option value="'.$va["db_col"].'">'.$val.'</option>';								
							$gC[$c-2] .= '</select>';
							

							$gO[$c-2] .= '<select style="width:130px;" class="form-control" name="order[]">';
							foreach($order as $ord){
								if($orders[$c-2]==$ord){
									$gO[$c-2] .= '<option selected="selected" value="'.$ord.'">'.$ord.'</option>';
								}else{
									$gO[$c-2] .= '<option value="'.$ord.'">'.$ord.'</option>';
								}
							}
							$gO[$c-2] .= '</select>';

							for($ii=0; $ii<1; $ii++){
								
								$gW[$c-2] .= '<select id="select'.($c-2).'" onchange="return isBetween('.($c-2).');" class="form-control" name="sym[]" style="width: 130px">';
								
								foreach($symbols[$data_type] as $k => $kk){
									if($syms[$c-2] == $kk)
										$gW[$c-2] .= '<option selected="selected" value="'.$kk.'">'.$k.'</option>';
									else
										$gW[$c-2] .= '<option value="'.$kk.'">'.$k.'</option>';
								}								
								
								$gW[$c-2] .= '</select>';
								
								$gV[$c-2] .= '<input type="hidden" name="dat_typ[]" value="'.$data_type.'"/>';
								$gV[$c-2] .= '<input type="'.$data_type.'" name="cit[]" value="'.$cits[$c-2].'" class="form-control" style="max-width:130px">';
								
							}
							

							if($data_type == "number" || $data_type == "date"){
								$gB[$c-2] .= '<div id="between'.($c-2).'">';

								
								
								$gA[$c-2] .= '&nbsp; AND &nbsp;';
								

								
								$gB[$c-2] .= '<input type="'.$data_type.'" name="bet[]" value="'.$bets[$c-2].'" class="form-control" style="max-width:130px">';
								$gB[$c-2] .= '</div>';
								
							}else{
								$gB[$c-2] .= '<input type="hidden" name="bet[]"/>';	
							}

						}	
					}
				}		
			}else{
				echo '<input type="hidden" name="table" value="'.$tableName.'">';
				echo '<input type="hidden" name="tab" value="'.$values.'">';
			}	

					
		}

		echo '<div class="clearfix"></div> ';
	echo '</div></div>';


		echo '<div style="">';
		echo '<table>';

		echo '<tr><td>&nbsp;</td></tr>';

		echo '</table>';
		echo '</div>';


		echo '<div style="background-color:white; border:1px solid black; position: relative; min-height: 200px;">';

		echo '<div style="top:2px; left:0px; padding-top: 51px; height:200px; position: absolute; background-color: rgba(255,255,255);z-index: 9;">';
		echo '<table border="1" style="" id="ac1" >';
			echo '';
		foreach($fields as $field => $field_value){
			if($field_value != 'And'){
				if($field_value == "Between") $field_value = "And";
				echo '<tr style="height: 24px;"><td>&nbsp;'.$field_value.'&nbsp;</td></tr>';
			}
		}
		echo '</table>';
		echo '</div>';

		echo '<div style="position: absolute;top: 0; left: 10px; z-index: 10">';
		echo '<div class="pull-left" style="margin:2px 10px;"><b>'.$tableName.' Report Columns:</b></div>';	
		echo '<div class="clearfix"></div>';				
		echo '<div class="pull-left" style="margin:2px 10px;"> Total Rows To return: &nbsp; </div>';
		echo '<div class="pull-left"  style="margin:2px 10px;"> <input name="to_return" type="number" value="'.$to_return.'" placeholder="All" class="form-control"  style="width:80px"  /></div>';
		echo '</div>';

		echo '<div style="overflow: auto;min-height: 170px; margin-top: 52px;">';
		echo '<table border="1" style="background-color:white" id="ac">';
		
		echo '<tr>';
		echo '<td></td>';
		echo '<td colspan="'.(count($gC)).'">';
		echo '</td></tr>';
		
		foreach($fields as $field => $field_value){
			if($field_value != 'And'){
				echo '<tr>';
				if($field_value == "Between") $field_value = "And";
				echo '<td>&nbsp;'.$field_value.'&nbsp;</td>';
				echo '<td style="max-height:24px;">'.implode('</td><td>', $$field).'</td>';
				echo '</tr>';
			}
		}
		echo '</table>';
		echo '</div>';

		?>
		<script type="text/javascript">
			function isBetween(id){
				var select = document.getElementById("select"+id);
				var between = document.getElementById("between"+id);
				
				if(select.value != "between"){
					between.style = "display:none;";
				}else{
					between.style = "display:block;";
				}
			}
		</script>
		<?php
		echo '<div class="clearfix"></div>';

		echo '';
		echo '</div>';
		
	}/////////////////
		/*
		echo '<br/>';
		echo '<div class="pull-left">';
		echo '<b>Report Format: &nbsp; &nbsp; </b>';
		echo '</div>';

		//echo '<div class="col-lg-3">';
		echo '<div class="pull-left">';
		echo '<select class="form-control">';
		foreach($report_format as $sym){
			echo '<option>'.$sym.'</option>';
		}
		echo '</select>';
		echo '</div>';
		//echo '</div>';

		echo '<div class="col-lg-1">';
		echo '</div>';
		*/

		echo '<br/>';

		if(portion(3)){		
			echo '<div class="pull-left">';
			echo '<button type="submit" class="btn btn-success" name="generate_report"> &nbsp; Generate Report &nbsp; </button>';
			echo '</div>';

			if(isset($_POST['generate_report'])){
				echo '<div class="pull-left"> &nbsp; <b>(Columns: <b>'.number_format(count($cols_to_use)).'</b>, Records: <b>'.number_format($db->num_rows()).'</b>)</b></div>';
			}
			echo '<div class="clearfix"></div>';
		}

		if(isset($_POST['generate_report'])){
			
		}

		echo '</form>';
		//echo '</div>';
		if(isset($_POST['generate_report'])){
			$db_values = array();

			$db_values[] = $cols_to_use;

			//echo count($cols_to_use);
			$re = array();
			for($j=0; $j<count($select[0]); $j++){
				
				for($i=0; $i<count($cols_to_use); $i++){
					//echo '<b>To:'.$cols_to_use[$i].'</b><br/>';				
					foreach($requiredReports[$table] as $a => $b){

						if(is_array($b)){
							$col = $b[$cols_to_use[$i]]['db_col'];							
							$data_type = $b[$cols_to_use[$i]]['data_type'];
							$default = $b[$cols_to_use[$i]]['default'];
							$match_to = $b[$cols_to_use[$i]]['match_to'];
							$method_caller = $b[$cols_to_use[$i]]['method_caller'];
							$sum = $b[$cols_to_use[$i]]['sum'];
							
							$col_value = ($select[0][$j][$col]);

							//------------------- METHOD CALLER -----------------
							if(!empty($method_caller)){

								$class_name = $method_caller[0];
								$method_name = $method_caller[1];
								if(class_exists($class_name)){									
									$class = new $class_name;									
									if(is_callable([$class, $method_name])){
										$col_value = $class->$method_name($col_value);	
									}else{
										$col_value = '-2';
									}									
								}else{
									$col_value = '-1';
								}
							}else if(!empty($sum)){	//------------ SUM -----------	 			
								$col_value = $this->total($sum[0], $sum[1], $col_value);
							}else if(!empty($match_to)){	//------------ MATCH TO -----------	 			
								$col_value = $this->rgf($match_to[0], $col_value, $match_to[1], $match_to[2]);
							}else{
								$col_value = $col_value;
							}

							if($data_type == "number"){
								$re[$j][$i] =  '<div style="text-align:right;width:100%;"> '.number_format($col_value).'</div>';
							}else if($data_type == "float"){
								$re[$j][$i] =  '<div style="text-align:right;width:100%;"> '.number_format($col_value, 2).'</div>';
							}else if($data_type == "text"){
								$re[$j][$i] = $col_value;
							}else if($data_type == "date"){
								$re[$j][$i] = Feedback::date_fm($col_value);
							}else if($data_type == "float"){
								$re[$j][$i] =  number_format($col_value,2);
							}else{
								$re[$j][$i] = $col_value;
							}

							//$re[$j][$i] = $col_value;
						}
					}					
				}
			}


			foreach($re as $x => $r){
				
				$db_values[] = $r;
			}

			
		}
		
		echo '<div>';
		if(isset($_POST['generate_report'])){

			//echo '<br/><b><h3>'.$tableName.' Report Successfully Generated</h3></b> <br/>(Columns: <b>'.number_format(count($cols_to_use)).'</b>, Records: <b>'.number_format($db->num_rows()).'</b>)';
			echo '<br/>';

			$t = new TableCreatorReport();
			$heading = $tableName; //CHANGE
			$t->open($this->full_name(user_id()), $heading);
			$t->thd($db_values);
			$t->close();
			echo $t->results();
			echo '<br/>';
			$e = new Exporter();
			echo $e->getDisplay($heading, $t->results());
		}
		echo '</div>';
		echo '<br/><br/>';

	}
	
	
}