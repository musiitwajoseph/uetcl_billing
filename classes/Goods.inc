<?php
Class Goods extends BeforeAndAfter{
	public $page = "USERS";
	
	public function __construct(){
		$access = new AccessRights();
		//$access->pageAccess(user_id(), $this->page, 'V');
	}
	
	public static function getLinks(){
		$page = "USERS";
		$links = array(
		
      array(
        "link_name"=>"Goods & Services COMMODITY CODE or CATEGORY ID", 
        "link_address"=>"goods/goods-and-services",
        "link_icon"=>"fa-table",
        "link_page"=>$page,
        "link_right"=>"V",
      ),
      array(
        "link_name"=>"Import Goods & Services", 
        "link_address"=>"goods/import-goods-and-services",
        "link_icon"=>"fa-upload",
        "link_page"=>$page,
        "link_right"=>"V",
      ),
			
			
			
		);
		
		return $links;
	}
	


	
	
public function deleteCategoryAction(){
    $id = portion(3);
    $this->deletor("dictionary_category", "id",  $id, 'dictionary/category');
  }
  
  
 public function importGoodsAndServicesAction(){
    $db = new Db();
    $time = time();
    $user = user_id();
 
    if (isset($_POST['send'])) {
        $errors = array();
     
      $filename=$_FILES["file"]["tmp_name"];
      if($_FILES["file"]["size"] > 0){

        $valid_name = "Segment Code";
        $file = fopen($filename, "r");
        while (($emapData = fgetcsv($file, 10000, ",")) !== FALSE){
          set_time_limit(0);
          $count++;
          $col_1 = @trim(str_replace("'","\'",strip_tags($emapData[0])));
          $col_2 = @trim(str_replace("'","\'",strip_tags($emapData[1])));
          $col_3 = @trim(str_replace("'","\'",strip_tags($emapData[2])));
          $col_4 = @trim(str_replace("'","\'",strip_tags($emapData[3])));
          $col_5 = @trim(str_replace("'","\'",strip_tags($emapData[4])));
          $col_6 = @trim(str_replace("'","\'",strip_tags($emapData[5])));
          $col_7 = @trim(str_replace("'","\'",strip_tags($emapData[6])));
          $col_8 = @trim(str_replace("'","\'",strip_tags($emapData[7])));
          $col_9 = @trim(str_replace("'","\'",strip_tags($emapData[8])));
          //$col_10 = @trim(str_replace("'","\'",strip_tags($emapData[9]))); 
         
       

          // if($count == 2){
          //  //echo "$col_1 is equal to $valid_name";
          //  if($col_1 != $valid_name){
          //    $errors[] = "Invalid Template";
          //  }
          // }
          
          
          echo $db->error();
        }
      
        fclose($file);

        if(empty($errors)){
          $count = 0;
          $db = new Db();
          $file = fopen($filename, "r");
          while (($emapData = fgetcsv($file, 10000, ",")) !== FALSE){

            set_time_limit(0);
            $count++;
            if($count > 2){
                $col_1 = @trim(str_replace("'","\'",strip_tags($emapData[0])));
                $col_2 = @trim(str_replace("'","\'",strip_tags($emapData[1])));
                $col_3 = @trim(str_replace("'","\'",strip_tags($emapData[2])));
                $col_4 = @trim(str_replace("'","\'",strip_tags($emapData[3])));
                $col_5 = @trim(str_replace("'","\'",strip_tags($emapData[4])));
                $col_6 = @trim(str_replace("'","\'",strip_tags($emapData[5])));
                $col_7 = @trim(str_replace("'","\'",strip_tags($emapData[6])));
                $col_8 = @trim(str_replace("'","\'",strip_tags($emapData[7])));
                $col_9 = @trim(str_replace("'","\'",strip_tags($emapData[8])));
                //$col_10 = @trim(str_replace("'","\'",strip_tags($emapData[9])));
            
           
            $db = new Db();
            $db->insert("good_services",["gs_segment_code"=>$col_1,"gs_segment_name"=>$col_2,"gs_family_code"=>$col_3,"gs_family_name"=>$col_4,"gs_class_code"=>$col_5,"gs_class_name"=>$col_6,"gs_commodity_code"=>$col_7,"gs_commodity_name"=>$col_8,"gs_service"=>$col_9,"gs_date_added"=>$time,"gs_added_by"=>$user]);
          
            
          }

          }

          if(!$db->error()){
            FeedBack::success();
            FeedBack::refresh(3, return_url().'goods/goods-and-services');
          }else{
            FeedBack::errors($errors);
          }
        }else{
          FeedBack::errors($errors);
        }
      }else{ 
        $errors[] = "Please Attach file";
      }

    
      }

   echo '<form action="" method="post" enctype="multipart/form-data">';
  
    
    echo '<div class="col-md-3">';
    echo '<label>Attach Goods/Services File</label>';
    echo '<input type="file" name="file">';
    echo '</div>';

    echo '<div class="col-md-3">';
    echo '<br/>';
    echo '<button name="send" type="submit"><i class="fa fa-fw fa-plane"></i> Upload</button>';
    echo '</div>';
    echo '</form>';
  
  }
 public function deleteDictionaryAction(){
     $id = portion(3);
    $this->deletor("dictionary", "d_id",  $id, 'dictionary/dictionary');
  }
  
  
 public function goodsAndServicesAction(){
    echo '<h4>';
    echo 'Segments';
    echo '</h4>';
   $access = new AccessRights();
      $db = new Db();
       $select = $db->select("SELECT distinct gs_segment_code,gs_segment_name FROM good_services ORDER BY gs_segment_code ASC");
      //print_r($select);
      if(!$select){
        $db->error();
      }else{
        echo '<table cellspacing="0" cellpadding="2" border="1" width="100%" id="table">';
        echo '<thead>';
        echo '<tr>';
        echo '<th width="30px">No.</th>';
        echo '<th>Segment Code</th>';
        echo '<th>Segment Name</th>';
        echo '<th>Total Segments</th>'; 
        
        if($access->sectionAccess(user_id(), $this->page, 'E') || $access->sectionAccess(user_id(), $this->page, 'D')){
          echo '<th width="100px">Action</th>';
        }
        echo '</tr>';
        echo '</thead>';
        
        $i=1;
        echo '<tbody>';
        foreach($select[0] as $row){
          extract($row);
          echo '<tr>';
          echo '<td><center>'.($i++).'.</center></td>';
          echo '<td>'.($gs_segment_code).'</td>';
          echo '<td>'.($gs_segment_name).'</td>';
          echo '<td>'.$this->total("good_services", "gs_segment_code", $gs_segment_code).'</td>';
          
          if($access->sectionAccess(user_id(), $this->page, 'E') || $access->sectionAccess(user_id(), $this->page, 'D')){
          
            echo '<td>';
            if($access->sectionAccess(user_id(), $this->page, 'E'))
              
            echo '<a class="btn btn-success btn-xs" href="'.return_url().'goods/Families/'.$gs_segment_code.'"><i class="fa fa-eye"></i> View</a>';
            echo '</td>';
          }
          echo '</tr>';
         
        }
        echo '</tbody>';
        
        echo '</table>';
        
      
        
      }
 }
 public function familiesAction(){
   $access = new AccessRights();
      $id = portion(3);
      $db = new Db();
       $select = $db->select("SELECT distinct gs_family_code,gs_family_name, gs_segment_code FROM good_services WHERE gs_segment_code ='$id'");
      //print_r($select);

    echo '<h4>';
    echo '<a href="'.return_url().'goods/goods-and-services">Segments</a>';
    echo '<i class="fa fa-fw fa-arrow-right"></i>';
    echo 'Family';
    echo '</h4>';

      if(!$select){
        $db->error();
      }else{
        echo '<table cellspacing="0" cellpadding="2" border="1" width="100%" id="table">';
        echo '<thead>';
        echo '<tr>';
        echo '<th width="30px">No.</th>';
        echo '<th>Family Code</th>';
        echo '<th>Family Name</th>';
        echo '<th>Total Families</th>';
        
        
        if($access->sectionAccess(user_id(), $this->page, 'E') || $access->sectionAccess(user_id(), $this->page, 'D')){
          echo '<th width="100px">Action</th>';
        }
        echo '</tr>';
        echo '</thead>';
        
        $i=1;
        echo '<tbody>';
        foreach($select[0] as $row){
          extract($row);
          echo '<tr>';
          echo '<td><center>'.($i++).'.</center></td>';
          echo '<td>'.($gs_family_code).'</td>';
          echo '<td>'.($gs_family_name).'</td>';
          echo '<td>'.$this->total("good_services", "gs_family_code", $gs_family_code).'</td>';
          
          if($access->sectionAccess(user_id(), $this->page, 'E') || $access->sectionAccess(user_id(), $this->page, 'D')){
          
            echo '<td>';
            if($access->sectionAccess(user_id(), $this->page, 'E'))
              
            echo '<a class="btn btn-success btn-xs" href="'.return_url().'goods/Classes/'.$gs_family_code.'/'.$gs_segment_code.'"><i class="fa fa-eye"></i> View</a>';
            echo '</td>';
          }
          echo '</tr>';
         
        }
        echo '</tbody>';
        
        echo '</table>';
        
      
        
      }
 }
  public function ClassesAction(){
   $access = new AccessRights();
      $id = portion(3);
      $db = new Db();
       $select = $db->select("SELECT distinct gs_class_code,gs_class_name,gs_segment_code,gs_family_code FROM good_services WHERE gs_family_code ='$id'");
      //print_r($select);


    echo '<h4>';
    echo '<a href="'.return_url().'goods/goods-and-services">Segments</a>';
    echo '<i class="fa fa-fw fa-arrow-right"></i>';
    echo '<a href="'.return_url().'goods/Families/'.portion(4).'">Family: '.portion(4).'</a>';
    echo '<i class="fa fa-fw fa-arrow-right"></i>';
    echo 'Class';
    echo '</h4>';

      if(!$select){
        $db->error();
      }else{
        echo '<table cellspacing="0" cellpadding="2" border="1" width="100%" id="table">';
        echo '<thead>';
        echo '<tr>';
        echo '<th width="30px">No.</th>';
        echo '<th>Class Code</th>';
        echo '<th>Class Name</th>';
        echo '<th>Total Classes</th>';
        // echo '<th>Family Name</th>';
        // echo '<th>Class Name</th>';
        // echo '<th>Class Name</th>';
        // echo '<th>Commodity Code</th>';
        // echo '<th>Commodity Name</th>';
        // echo '<th>Is it a Services</th>'; 
        
        if($access->sectionAccess(user_id(), $this->page, 'E') || $access->sectionAccess(user_id(), $this->page, 'D')){
          echo '<th width="100px">Action</th>';
        }
        echo '</tr>';
        echo '</thead>';
        
        $i=1;
        echo '<tbody>';
        foreach($select[0] as $row){
          extract($row);
          echo '<tr>';
          echo '<td><center>'.($i++).'.</center></td>';
          echo '<td>'.($gs_class_code).'</td>';
          echo '<td>'.($gs_class_name).'</td>';
          echo '<td>'.$this->total("good_services", "gs_class_code", $gs_class_code).'</td>';
          //echo '<td>'.($gs_family_name).'</td>';
          // echo '<td>'.$gs_class_code.'</td>';
          // echo '<td>'.$gs_class_name.'</td>';
          // echo '<td>'.$gs_commodity_code.'</td>';
          // echo '<td>'.$gs_commodity_name.'</td>';
          // echo '<td>'.$gs_service.'</td>';
          //echo '<td>'.$gs_service.'</td>';
          if($access->sectionAccess(user_id(), $this->page, 'E') || $access->sectionAccess(user_id(), $this->page, 'D')){
          
            echo '<td>';
            if($access->sectionAccess(user_id(), $this->page, 'E'))
              
            echo '<a class="btn btn-success btn-xs" href="'.return_url().'goods/Commodities/'.$gs_class_code.'/'.$gs_segment_code.'/'.$gs_family_code.'"><i class="fa fa-eye"></i> View</a>';
            echo '</td>';
          }
          echo '</tr>';
         
        }
        echo '</tbody>';
        
        echo '</table>';
        
      
        
      }
 }
 public function CommoditiesAction(){
   $access = new AccessRights();
      $id = portion(3);
      $db = new Db();
       $select = $db->select("SELECT distinct gs_commodity_name,gs_commodity_code,gs_family_name, gs_class_code,gs_class_name,gs_service FROM good_services WHERE gs_class_code ='$id'");
      //print_r($select);

    echo '<h4>';
    echo '<a href="'.return_url().'goods/goods-and-services">Segments</a>';
    echo '<i class="fa fa-fw fa-arrow-right"></i>';
    echo '<a href="'.return_url().'goods/Families/'.portion(4).'">Family: '.portion(4).'</a>';
    echo '<i class="fa fa-fw fa-arrow-right"></i>';
    echo '<a href="'.return_url().'goods/Classes/'.portion(5).'/'.portion(4).'">Class: '.portion(5).'</a>';
    echo '<i class="fa fa-fw fa-arrow-right"></i>';
    echo 'Commodity';
    echo '</h4>';

      if(!$select){
        $db->error();
      }else{
        echo '<table cellspacing="0" cellpadding="2" border="1" width="100%" id="table">';
        echo '<thead>';
        echo '<tr>';
        echo '<th width="30px">No.</th>';
        echo '<th>Family Name</th>';
        echo '<th>Class Name</th>';
        echo '<th>Class Name</th>';
        echo '<th>Commodity Code</th>';
        echo '<th>Commodity Name</th>';
        echo '<th>Is it a Services</th>'; 

        //echo '<th>Total Commodities</th>';
       
        
        // if($access->sectionAccess(user_id(), $this->page, 'E') || $access->sectionAccess(user_id(), $this->page, 'D')){
        //   echo '<th width="100px">Action</th>';
        // }
        echo '</tr>';
        echo '</thead>';
        
        $i=1;
        echo '<tbody>';
        foreach($select[0] as $row){
          extract($row);
          echo '<tr>';
          echo '<td><center>'.($i++).'.</center></td>';
          // echo '<td>'.($gs_commodity_code).'</td>';
          // echo '<td>'.($gs_commodity_name).'</td>';
          echo '<td>'.($gs_family_name).'</td>';
          echo '<td>'.$gs_class_code.'</td>';
          echo '<td>'.$gs_class_name.'</td>';
          echo '<td style="font-weight:bold;color:blue;">'.$gs_commodity_code.'</td>';
          echo '<td style="font-weight:bold;color:blue;">'.$gs_commodity_name.'</td>';
          echo '<td>'.$gs_service.'</td>';
          //echo '<td>'.$gs_service.'</td>';
          //echo '<td>'.$this->total("good_services", "gs_commodity_code", $gs_commodity_code).'</td>';
        
          // if($access->sectionAccess(user_id(), $this->page, 'E') || $access->sectionAccess(user_id(), $this->page, 'D')){
          
          //   echo '<td>';
          //   if($access->sectionAccess(user_id(), $this->page, 'E'))
              
          //   echo '<a class="btn btn-success btn-xs" href="'.return_url().'goods/Commodities/'.$gs_class_code.'"><i class="fa fa-eye"></i> View</a>';
          //   echo '</td>';
          // }
          echo '</tr>';
         
        }
        echo '</tbody>';
        
        echo '</table>';
        
      
        
      }
 }
}