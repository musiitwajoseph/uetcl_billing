<?php
class BeforeAndAfter{
	//public $user_id = $_SESSION['USER_ID'];
	
	public $show1 = "Active";
	public $show2 = "Inactive";
	
	public $show11 = "Active";
	public $show22 = "Inactive";

	public function __call($name, $args){


		//run code before
		$method = "{$name}Action";
		if(method_exists($this, $method)){
			if($this->before() !== false){
				
				call_user_func_array([$this, $method],$args);
				$this->after();
			}
		}else{
			echo $method.' Method doesnot exists';
		}
		
	}
	
	protected function deletor($table, $column,  $id, $return_to){
		$db = new Db();
		
		$id = portion(3);
		$sql = "DELETE FROM $table WHERE $column = '$id'";
		
		AuditTrail::registerTrail($sql, $id,  $table, $column);

		$delete = $db->query($sql);
		
		if($delete){
			FeedBack::success('Deleting. Please wait ...');
			FeedBack::refresh(1,return_url().$return_to);
		}else{
			FeedBack::error('Not Deleted '.$db->error());
		}
	}
	
	
	public function action($type, $link, $word = ""){
		
		$type = strtolower($type);
		
		if($type == "edit")
			return '<a href="'.return_url().$link.'" class="btn btn-xs btn-success" style="margin:0 2px;">'.$word.'</a>';
		
		if($type == "delete")
			return '<a href="'.return_url().$link.'" class="btn btn-xs btn-danger" style="margin:0 2px;">'.$word.'</a>';
		
		if($type == "view")
			return '<a href="'.return_url().$link.'" class="btn btn-xs btn-warning"> '.$word.'</a>';
			
		return 'none';
	}
	protected function before(){
		
		$link_part0 = str_replace('-', '-', ucwords(portion(1)));
		$link_part5 = str_replace(' ', '',ucwords(str_replace('-', ' ', portion(1))));
		$link_part1 = portion(2);
		
		$link_part0 = (empty($link_part0))?"Dashboard":$link_part0;
		$link_part5 = (empty($link_part5))?"Dashboard":$link_part5;
		$link_part1 = (empty($link_part1))?"index":$link_part1;
		
		$title = ucwords(strtolower(str_replace('-', ' ', $link_part1)));
		echo '<div class="page-header" style="margin-top:0px;margin-bottom:0px;padding-top:0px;"><span style="font-weight:bold; font-size:1.5em;margin:bottom:5px;padding-bottom:5px;">'.str_replace(' Of ',' of ',$title) .'</span>';
		
		echo '<ul class="inner-menu">';
				
		$class_name = ucwords($link_part5);
		$h = new $class_name;
		
		if(!empty($h->getLinks())){
			foreach($h->getLinks() as $link){
				extract($link);								
				$a = new AccessRights();
				if($a->sectionAccess(user_id(), $link_page, $link_right))
				{				
					echo '<li>';
					
					if(strtolower($link_part0).'/'.$link_part1 == $link_address)
						echo '<i class="fa fa-fx '.$link_icon.'"></i> '.$link_name;
					else
						echo '<a href="'.return_url().$link_address.'"><i class="fa fa-fx '.$link_icon.'"></i> '.$link_name.'</a>';
					
					echo '</li>';
				}
			}
		}
		
		echo '</ul>';
		echo '</div>';
	}
	
	public function full_name($id){
		$db = new Db();
		$row = $db->select("SELECT user_surname, user_othername FROM sysuser WHERE user_id = '$id'");
		$db->error();
		return ucwords(strtolower($row[0][0]['user_surname'].' '.$row[0][0]['user_othername']));
	}
	//return user field
	protected function ruf($id, $column){
		$db = new Db();
		$row = $db->select("SELECT $column FROM sysuser WHERE user_id = '$id'");
		echo $db->error();
		return $row[0][0][$column];
	}
	//return generic field from generic table
	public function rgf($table, $id, $look_up, $column){
		$db = new Db();
		$row = @$db->select("SELECT $column FROM $table WHERE $look_up = '$id' ");
		echo $db->error();
		return @$row[0][0][$column];
	}

	//return distinct field from generic table
	protected function rdfs($table, $column, $order=array()){
		$db = new Db();
		$row = $db->select("SELECT distinct $column FROM $table ");
		echo $db->error();
		return $row[0];
	}

	protected function hod($id){
		$db = new Db();
		$row = $db->select("SELECT user_department_id FROM sysuser WHERE user_id = '$id'");
		$user_department_id = $row[0][0]['user_department_id'];
		$hod = static_hod_id();
		$b = new Db();
		$bb = $b->select("SELECT user_id FROM sysuser WHERE user_designation = '$hod' AND user_department_id = '$user_department_id'");					
		extract($bb[0][0]);
		$user_id;
		return $user_id;
	}
	protected function dept_name($id){
		$db = new Db();
		$row = $db->select("SELECT dept_name FROM department WHERE dept_id = '$id'");
		echo $db->error();
		return $row[0][0]['dept_name'];
	}
	
	protected function sec_name($id){
		$db = new Db();
		$row = $db->select("SELECT section_name FROM section WHERE section_id = '$id'");
		echo $db->error();
		return $row[0][0]['section_name'];
	}
	
	protected function hod_id($id){
		$db = new Db();
		$row = $db->select("SELECT hod_user_id FROM hod WHERE hod_dept_id = '$id'");
		echo $db->error();
		return @$row[0][0]['hod_user_id'];
	}
	
	protected function total($table, $column="", $column_value=""){
		//echo "SELECT * FROM $table WHERE $column = '$column_value'";
		$db = new Db();
		if(empty($column))
			$row = $db->select("SELECT * FROM $table");
		else
			$row = $db->select("SELECT * FROM $table WHERE $column = '$column_value'");
			
		echo $db->error();
		return @$db->num_rows();
	}
	
	
	protected function branch_name($id){
		$db = new Db();
		$row = $db->select("SELECT branch_name FROM branch WHERE branch_id = '$id'");
		echo $db->error();
		return $row[0][0]['branch_name'];
	}
	
	protected function grade($id){
		$db = new Db();
		$row = $db->select("SELECT grade FROM users WHERE user_id = $id limit 1");
		
		return $row[0]['grade'];
	}
	protected function staff_code($id){
		$db = new Db();
		$row = $db->select("SELECT staff_code FROM users WHERE user_id = $id limit 1");
		
		return $row[0]['staff_code'];
	}
	
	
	protected function role_id($id){
		$db = new Db();
		$row = $db->select("SELECT users.role_id as role_id FROM users, roles WHERE users.user_id = $id AND users.role_id = roles.role_id limit 1");
		
		return $row[0]['role_id'];
	}
	
	protected function role_name($id){
		$db = new Db();
		$row = $db->select("SELECT role_name FROM users, roles WHERE users.user_id = $id AND users.role_id = roles.role_id limit 1");
		
		return $row[0]['role_name'];
	}
	
	protected function designation_name($id){
		$db = new Db();
		$row = $db->select("SELECT designation_name FROM designation WHERE designation_id = '$id'");
		echo $db->error();
		return $row[0][0]['designation_name'];
	}
	
	protected function requestor_id($id){
		$db = new Db();
		$row = $db->select("SELECT vr_requestor_id FROM vehicle_request WHERE vr_id = '$id'");
		echo $db->error();
		return $row[0][0]['vr_requestor_id'];
	}
	
	public function rejector($table, $column_name=array(), $column_prefix=array()){
		$u = new Db();
		
		foreach ($column_name as $key => $value) {
			$column_name_and_value = $key.'='.$value.'';
		}
		
		foreach ($column_prefix as $key => $value) {
			$column_prefix_db = $key;
			$column_count = $value;
		}

		for($i=0; $i<$column_count-1; $i++){
			$lev[] = ' '.$column_prefix_db.($i+1)." = NULL ";
		}
		$levs = implode(" , ", $lev);

		$sql = "UPDATE $table SET $levs WHERE $column_name_and_value";

		$db = new Db();
		$update = $db->query($sql);
	}

	protected function department($id, $table = false){
		$db = new Db();
		

		if($table==true){
			$row = $db->select("SELECT dp_name FROM department WHERE dp_id = '$id'");
		}else{
			$row = $db->select("SELECT dp_name FROM users, department WHERE users.user_id = $id AND users.user_dp_id = department.dp_id limit 1");
		}
		
		return $row[0]['dp_name'];
	}
	
	protected function check_de_status($user_id, $type="SMS"){
		//$db = new Db();
		//SELECT de_id, de_status, de_name, de_user_id FROM disable_enable WHERE 1
		//$row = $db->select("SELECT de_status FROM disable_enable WHERE de_user_id = $user_id AND de_name = '$type' limit 1");
		
		//if($db->num_rows()==0){
			//return 1;
		//}

		//return !($row[0]['de_status']);
		return 1;
	}

		
	public function department_id($id){
		$db = new Db();
		$row = $db->select("SELECT user_dp_id FROM users WHERE users.user_id = $id limit 1");
		
		return @$row[0]['user_dp_id'];
	}

	public function cost_center1($id){
		$db = new Db();
		$row = $db->select("SELECT cost_center_name FROM cost_center WHERE cost_center_id = '$id'");
		
		return @$row[0][0]['cost_center_name'];
	}

	protected function isThere($table, $params = array()){
		$db = new Db();

		foreach ($params as $key => $value) {
			$vals[] = "$key='$value'";
		}

		$all_vals = implode(' AND ', $vals);

		$select = $db->select("SELECT * FROM $table WHERE $all_vals;");

		if($db->num_rows()){
			return true;
		}else{
			return false;
		}

	}


	protected function isThereEdit($table, $params = array()){
		
		$db = new Db();
		$i=0;
		$all = count($params);

		foreach ($params as $key => $value) {
			$i++;
			
			if($all == $i){
				$vals[] = "$key != '$value'";
			}else{
				$vals[] = "$key = '$value'";
			}
		}

		$all_vals = implode(' AND ', $vals);
		//echo "SELECT * FROM $table WHERE $all_vals;";
		$select = $db->select("SELECT * FROM $table WHERE $all_vals;");

		if($db->num_rows()){
			return true;
		}else{
			return false;
		}

	}

	protected function slashValuesTrim(&$params){
		foreach($params as &$var){
			is_array($var)? slashValues($var) : $var = trim(addslashes($var));
			unset($var);
		}
	}


	protected function stripValues(&$params){
		foreach($params as &$var){
			is_array($var)? html($var) : $var = trim(stripslashes($var));
			unset($var);
		}
	}
	
	protected function permitExpChecker($driver_id){
		$n = new Db();
		$select = $n->select("SELECT dp_expiry_date FROM driver_permit WHERE dp_driver_id = '$driver_id'");

		$time = $select[0][0]['dp_expiry_date'];

		$remaining = $time  - time();
		$remaining = (($remaining/(24*60*60))); 
		if($remaining < 0) $remaining = 0;
		else $remaining = ceil($remaining);
		
		return $remaining;
	}

	protected function vehicleReturnDate($vehicle_request){

		$n = new Db();
		$nn = $n->select("SELECT * FROM route WHERE route_vehicle_request_no = '$vehicle_request' ORDER BY route_return_date DESC");
		echo $n->error();
		return $nn[0][0]['route_return_date'];

	}

	protected function poolTotalDays($vr_number){
		$n = new Db();
		$start_day = $end_day = 0; 
		$nn = $n->select("SELECT * FROM route WHERE route_vehicle_request_no = '$vr_number'");
		$i=1;
		foreach($nn[0] as $nnn){
			extract($nnn);
			if(empty($route_orign) && empty($route_destination)){

			}else{
				if($i==1){
					$start_day = strtotime($route_departure_date);
				}				
				$end_day = strtotime($route_return_date);	
				$i++;			
			}			
		}

		return $total_days = ceil(($end_day-$start_day)/(24*60*60))+1;
	}

	protected function poolReturnDate($vr_number){
		$n = new Db();
		$start_day = $end_day = 0; 
		$nn = $n->select("SELECT * FROM route WHERE route_vehicle_request_no = '$vr_number'");
		$i=1;
		foreach($nn[0] as $nnn){
			extract($nnn);
			if(empty($route_orign) && empty($route_destination)){

			}else{
				if($i==1){
					$start_day = strtotime($route_departure_date);
				}				
				$end_day = Feedback::date_s(strtotime($route_return_date));	
				$i++;			
			}			
		}

		return $end_day;
	}

	protected function poolDepartureDate($vr_number){
		$n = new Db();
		$start_day = $end_day = 0; 
		$nn = $n->select("SELECT * FROM route WHERE route_vehicle_request_no = '$vr_number'");
		$i=1;
		foreach($nn[0] as $nnn){
			extract($nnn);
			if(empty($route_orign) && empty($route_destination)){

			}else{
				if($i==1){
					$start_day = Feedback::date_s(strtotime($route_departure_date));
				}				
				$end_day = strtotime($route_return_date);	
				$i++;			
			}			
		}

		return $start_day;
	}

	protected function poolTotalDistance($vr_number){
		$n = new Db();
		$distance = 0; 
		$nn = $n->select("SELECT * FROM route WHERE route_vehicle_request_no = '$vr_number'");
		$i=1;
		foreach($nn[0] as $nnn){
			extract($nnn);
			if(empty($route_orign) && empty($route_destination)){

			}else{
				$distance += $route_distance;
			}		
		}

		return $distance;	
	}


	protected function poolTotalFuelAmount($pfr, $fuel = 1){
		$n = new Db();
		$i=1;
		$db = new Db();

		$select = $db->select("SELECT * FROM pool_fuel_request WHERE pfr_id = '$id' ");
		extract($select[0][0]);

		$pfr_vehicle_request_id = $this->rgf("pool_fuel_request", $pfr, "pfr_id", "pfr_vehicle_request_id");

		$vdtr_vehicle_id = $this->rgf("vehicle_driver_to_request", $pfr_vehicle_request_id, "vdtr_vehicle_request_id", "vdtr_vehicle_id");
	
		$vr_number = $this->rgf('vehicle_request', $pfr_vehicle_request_id, 'vr_id', 'vr_number');

		$consumption_rate = $this->rgf('vehicle', $vdtr_vehicle_id, 'vehicle_id', 'vehicle_fuel_consumption_rate');

		$product_id = $this->rgf('vehicle', $vdtr_vehicle_id, 'vehicle_id', 'vehicle_product_id');

		$product_name = $this->rgf('product', $product_id, 'pro_id', 'pro_name');
		$product_price = $this->rgf('product', $product_id, 'pro_id', 'pro_price');
		
		$nn = $n->select("SELECT * FROM route WHERE route_vehicle_request_no = '$vr_number'");
		foreach($nn[0] as $nnn){
			extract($nnn);

			if(empty($route_orign) && empty($route_destination)){

			}else{
				
				$litres = $route_distance*$consumption_rate;
				$total_litres += $litres;
			
				$total_distance += $route_distance;
				$price = (($route_distance*$consumption_rate)*$product_price);
				$total_price += $price;
				
			}
			
		}

		if($fuel==1){
			return number_format($total_litres, 2);
		}else{
			return $total_price;
		}
	}

	protected function poolTotalAmount($pfr){
		return $this->poolTotalFuelAmount($pfr, 0);
	}

	protected function poolTotalLitres($pfr){
		return $this->poolTotalFuelAmount($pfr, 1);
	}

	//----------------- POOL VEHICLE SERIAL NUMBERS ----------------------
	public function vehicle_request_number(){		
		$dm = date('y').date('m');
		$suffix = "POVR".$dm;
		$db = new Db();
		$select = $db->select("SELECT TOP 1 vr_number FROM vehicle_request ORDER BY vr_id DESC");
		extract($select[0][0]);
		$f = end(explode($suffix, $vr_number))+1;

		$number = $suffix.str_pad($f, 4, "0", STR_PAD_LEFT);
		return $number;	
	}

	public function return_vehicle_number(){
		$dm = date('y').date('m');
		$suffix = "POVT".$dm;
		$db = new Db();
		$select = $db->select("SELECT TOP 1 vdtr_return_number FROM vehicle_driver_to_request ORDER BY vdtr_id DESC");
		extract($select[0][0]);
		$f = end(explode($suffix, $vdtr_return_number))+1;

		$number = $suffix.str_pad($f, 4, "0", STR_PAD_LEFT);
		return $number;
	
	}
	//pool fuel request number 
	public function pfr(){
		$dm = date('y').date('m');
		$suffix = "POFR".$dm;
		$db = new Db();
		$select = $db->select("SELECT TOP 1 pfr_number FROM pool_fuel_request ORDER BY pfr_id DESC");
		extract($select[0][0]);
		$f = end(explode($suffix, $pfr_number))+1;

		$number = $suffix.str_pad($f, 4, "0", STR_PAD_LEFT);
		return $number;		
	}

	public function pfar(){		
		$dm = date('y').date('m');
		$suffix = "POVA".$dm;
		$db = new Db();
		$select = $db->select("SELECT pfar_number FROM pool_fuel_acc_req ORDER BY pfar_id DESC");
		extract($select[0][0]);
		$f = end(explode($suffix, $pfar_number))+1;

		$number = $suffix.str_pad($f, 4, "0", STR_PAD_LEFT);
		return $number;		
	}
	// ----------------------------------------------

	public function gro(){
		$dm = date('y').date('m');
		$suffix = "GRO".$dm;
		$db = new Db();
		$select = $db->select("SELECT TOP 1 ro_group_number FROM reload_order ORDER BY ro_id DESC");
		extract($select[0][0]);
		$f = end(explode($suffix, $ro_group_number))+1;

		$number = $suffix.str_pad($f, 4, "0", STR_PAD_LEFT);
		return $number;		
	}

	public function period($date){

		$mo = array(
			"JAN"=>"07",
			"FEB"=>"08",
			"MAR"=>"09",
			"APR"=>"10",
			"MAY"=>"11",
			"JUN"=>"12",
			"JUL"=>"01",
			"AUG"=>"02",
			"SEP"=>"03",
			"OCT"=>"04",
			"NOV"=>"05",
			"DEC"=>"06", 
		);
		$p = strtotime("+6 months", $date);
		return date("m/Y", $p);
	}

	protected function after(){
		
	}
}
?>