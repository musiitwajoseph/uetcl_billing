<?php 
Class TableCreatorSchedule{
	public $all = "";
	private $generator = "";
	private $tableName = "";

	Public function open($generator="", $tableName="", $classes=array(), $id="table"){
		$this->generator = $generator;
		$this->tableName = $tableName;
		$this->all .= '<style>
						#table{
						  border-spacing:0;
						  border-collapse:collapse;
						  width:100%;
						}
						#table tr{
						  transition:background-color 0.3s linear 0s;
						}
						#table tr:nth-child(odd){
						  background-color:#D2D2D2;
						}
						#table tr:nth-child(even){
						  background-color:#EBEBEB;
						}
						#table tr td{
						  padding:5px 2px 10px 2px;
						  font-size:12px;
						  
						}
						#table th{
						  color:#000;
						  background:#D3DCE3;
						  padding:5px 2px 10px 2px;
						  font-weight:bold;
						  font-size:13px; 
						  text-align:center;
						  
						  background:#336F89;
						  color:#fff;
						}
						#table tr:hover{
						  background-color:#E3F282;
						  transition:background-color 0.3s linear 0s;
						  color:#000;
						}
						</style>';

		$all_classes = "class = '";
		$all_ids = "";

		foreach ($classes as $key => $value) {
			$all_classes .= "$value ";
		}
		$all_classes .= "' ";

		$all_ids = " id='$id' ";

		$this->all .= '<table id="table" style="font-size:10px;" border="1" cellspacing="0" cellpadding="2"'.$all_classes.$all_ids.'>';

	}

	Public function close(){
		$this->all .= "</tbody></table>";
	}

	Public function th($ths=array()){
		$all_ths = "";
		foreach($ths as $th=>$values){			
			$this->all .= "<tr>";
			foreach($ths as $th=>$values){
				$this->all .= "<th>$th</th>";
			}
			$this->all .= "</tr>";
		}
		
	}

	Public function td($tds=array()){
		$all_tds = "";
		foreach($tds as $td=>$values){			
			$this->all .= "<tr>";
			foreach($tds as $td=>$values){
				$this->all .= "<td>$td</td>";
			}
			$this->all .= "</tr>";
		}
		
	}

	Public function thd($tds=array()){
		$all_tds = "";
		$i=0;
		foreach($tds as $td=>$values){
			$total = count($values);

			if($i>=1){
			
				if($i%2 == 0)
					$this->all .= "<tr style='background-color:#EBEBEB; $align'>";
				else
					$this->all .= "<tr style='background-color:#fff; $align' >";
				

				//$this->all .= "<tr>";
				//$this->all .= "<td>".($i)."</td>";
				$b = 0;
				foreach($values as $td=>$val){

					$align = "";
					if(is_int($val)||is_double($val)){
						$align = " text-align:right; ";
					}

					if($b == 5 || $b == 6 || $b == 7)
						$this->all .= "<td style='color:red;'>$val</td>";
					else
						$this->all .= "<td >$val</td>";

					$b++;
					
				}

				$this->all .= "</tr>";
			}else{
			
				$this->all .= "<thead><tr><td colspan='$total'>";
				$this->all .= strtoupper($this->tableName);
				$this->all .= '<br/>';
				$this->all .= 'Exported by: '.$this->generator;
				$this->all .= '<br/>';
				$this->all .= 'Date: '.date('Y m d h:i:s A');
				$this->all .= "</td></tr>";	
				
				$this->all .= $this->headersSchedule();

				$this->all .= "<tr>";
				//$this->all .="<th>No.</th>";
				foreach($values as $td=>$val){
					$this->all .= "<th style='background-color:#336F89; color:#fff;'>$val</th>";
				}
				$this->all .= "</tr></thead><tbody>";
			}
			
			$i++;
		}

		$this->all .= "</tbody>";
		
	}

	public function headersSchedule(){

		$t .= '<tr>';
		$t .= '<th rowspan="2"></th>';
		$t .= '<th rowspan="2"></th>';
		$t .= '<th colspan="3">Main Meter - Current Readings(MWh)</th>';
		$t .= '<th colspan="3">Main Meter - Previous Readings</th>';
		$t .= '<th colspan="3">Main Meter - Current Readings</th>';
		$t .= '<th colspan="8">Advance</th>';
		$t .= '</tr>';

		$t .= '<tr>';
		$t .= '<th colspan="3">Imports</th>';
		$t .= '<th colspan="3">Imports</th>';
		$t .= '<th colspan="3">Exports</th>';
		$t .= '<th colspan="4">Imports</th>';
		$t .= '<th colspan="4">Exports</th>';
		$t .= '</tr>';

		return $t;
	}
	public function title($t){
		return $t;
	}

	Public function results(){
		return $this->all;
	}	

}

?>