<?php
Class JVApprovalLevels extends BeforeAndAfter{
	public $page = "JV APPROVAL LEVELS";
	
	public function __construct(){
		$access = new AccessRights();
		//$access->pageAccess(user_id(), $this->page, 'V');
	}
	
	public function getLinks(){
		$page = "JV APPROVAL LEVELS";
		$links = array(
			
			array(
				"link_name"=>"Approval Levels", 
				"link_address"=>"jv-approval-levels/approval-levels",
				"link_icon"=>"fa-eye",
				"link_page"=>$page,
				"link_right"=>"V",
			)
		);
		
		return $links;
	}
	
	public function ApprovalLevelsAction(){
            $masterApprovalOrder = new MasterApprovalOrder();
            $masterApprovalOrder->order('JV', 'JV');

    
	}
	
	
}