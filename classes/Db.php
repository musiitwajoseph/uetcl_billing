
<?php    

class Db {
	// The database connection
	protected static $connection;
	public $number_of_rows;
	public $error = array();
	/**
	 * Connect to the database
	 * 
	 * @return bool false on failure / mysqli MySQLi object instance on success
	 */
	public function connect() {
		
		// Try and connect to the database
		if(!isset(self::$connection)) {
			// Load configuration as an array. Use the actual location of your configuration file
			// Put the configuration file outside of the document root
			
			//self::$connection = new PDO('sqlsrv:server=169.254.228.188;Database=I', 'admin124', '123');
			//$connection = 
			
			
			self::$connection = new PDO('sqlsrv:server=BILLING;Database=PRODUCTION', 'test', 'test',
				array(PDO::ATTR_ERRMODE => PDO::ERRMODE_WARNING));
		}
	
		// If connection was not successful, handle the error
		if(self::$connection === false) {
			// Handle error - notify administrator, log to a file, show an error screen, etc.
			return false;
		}
		return self::$connection;
	}
	
	/**
	 * Query the database
	 *
	 * @param $query The query string
	 * @return mixed The result of the mysqli::query() function
	 */
	
	
	public function query($query, $params = array()) {
		// Connect to the database
		$connection = $this -> connect();
		
		// Query the database
		//$result = $connection -> query($query);
		
		try{
			
			// Query the database
			$result = $connection -> prepare($query);	
			
			//binding params
			$x=1;
			foreach($params as $param=>&$value){
				
				$result->bindParam($param, $value);//, $this->type($value)
			}	
			$result->execute();
			
			$this->error = $result->errorInfo();
		}catch(PDOException $e){
			$this->error = $e->getMessage();
		}
		
		return $result;
	}
	
	private function type($value){
		$type = "";
		if(is_int($value[1])) {
			$type = PDO::PARAM_INT;
		} else if(is_bool($value[1])) {
			$type = PDO::PARAM_BOOL;
		} else if(is_null($value[1])) {
			$type = PDO::PARAM_NULL;
		} else {
			$type = PDO::PARAM_STR;
		}
		return $type;
	}
	
	/**
	 * Fetch rows from the database (SELECT query)
	 *
	 * @param $query The query string
	 * @return bool False on failure / array Database rows on success
	 */
	public function select($query, $params=array()) {

		$rows = array();
		$result = $this -> query($query, $params);
		if($result === false) {
			return false;
		}
		$x=0;
		while ($row = $result -> fetchAll(PDO::FETCH_ASSOC)) {
			$rows[] = $row;
			$x++;
		}
		$this->number_of_rows = count(@$rows[0]); 

		return $rows;
	}
	
	
	function insert($table, $params=array()) {
		$next_params = $params;
		try {

			//echo 'tota:'.count($params).'<br/>';
			$keys = array_keys($params);
			$fields = implode(", ", $keys);
			
			$values = ":" . implode(", :", $keys);
			
			$insert = "INSERT INTO $table ($fields) VALUES ($values)";
				
			$params = array();

			foreach ($next_params as $key => $value) {
				$params[':'.$key] = $value;
			}
			//echo '<br/>lllll tota:'.count($next_params).'<br/>';;
			
			$in = $this->query($insert, $params);

			if($table != "trail_of_users"){
				//AuditTrail::registerTrail($insert, $db_id="",  $table, implode(' , ', $params));
			}
					
			return $in; 
			
		} catch(PDOException $e) {
			$this->error =  'ERROR: ' . $e->getMessage();
		}
	}
	
	
	function update($table, $params=array(), $id = array()) {
		$next_params = $params;
		$next_id = $id;
		try {
			//echo 'tota:'.count($id).'<br/>';
			//print_r($id);
						
			$c = array();
			foreach($params as $i=>$value){
				$c[] = "$i=:$i";
			}
			
			$w = array();
			foreach($id as $i=>$value){
				$w[] = "$i=:$i";
			}
			
			$columns = implode(' , ', $c);
			$where = implode(' AND ', $w);
			$insert = "UPDATE $table SET $columns WHERE $where";
				
			$params = array();
			
			foreach ($next_params as $key => $value) {
				$params[':'.$key] = $value;
				//echo "<br/>params[':'.$key] = $value";
			}
			
			foreach ($next_id as $key => $value) {
				$params[':'.$key] = $value;
				//echo "<br/>params[':'.$key] = $value";
			}
			//echo '<br/>lllll tota:'.count($next_params).'<br/>';;
			
			$in = $this->query($insert, $params);

			if($table != "trail_of_users"){
				//AuditTrail::registerTrail($insert, $db_id="",  $table, implode(' , ', $params));
			}
					
			return $in; 
			
		} catch(PDOException $e) {
			$this->error =  'ERROR: ' . $e->getMessage();
		}
	}
	
	
	/**
	 * Fetch the last error from the database
	 * 
	 * @return string Database error message
	 */
	public function error() {
		if(empty((int)$this->error[0])) return '';
		return '<ol><li>'.implode('</li><li>', $this -> error).'</li></ol>';
	}
	public function last_id($table, $column){
		$connection = $this->connect();
		//return $connection->lastInsertId();
		$row = $this->select("SELECT MAX($column) AS max FROM $table");
		return $row[0][0]['max'];
	}
	public function trans($type){
		$connection = $this->connect();
		$type = strtolower($type);
		if($type == "begin"){
			$connection->beginTransation();
		}else if($type == "rollback"){
			$connection->rollBack();
		}else if($type == "commit"){
			$connection->commit();
		}
	}
	public function num_rows(){
		return $this->number_of_rows;
	}
}

$db = new Db();
//echo $db->quote("sdf sdf");

//$x = $db->insert("department",["dept_date_added"=>43324, "dept_name"=>"JOHN MALE Jseph","dept_added_by"=>11]);

//$x = $db->update("department", ["dept_date_added"=>110011], ['dept_id'=>20]);
//echo $db->error();

//$x = $db->select("SELECT * FROM department");
//echo '<pre>';
//print_r($x);
//echo '</pre>';
//echo $db->error();
?>
