<?php

function mytinytodo_add_css($src) {
	if ($src[0] == '/')
		$src = substr($src, 1);
	drupal_add_css($src, array('group' => CSS_DEFAULT));
}

function mytinytodo_add_js($src) {
	drupal_add_js($src);
}

function mytinytodo_add_library($module, $name) {
	global $base_path;

	$output = '';
	if ($library = drupal_get_library($module, $name)) {
		if ($library['js']) {
			foreach($library['js'] as $file => $info)
				mytinytodo_add_js($base_path . $file);
		}
		if ($library['css']) {
			foreach($library['css'] as $file => $info)
				mytinytodo_add_css($base_path . $file);
		}
	}
}

function theme_mytinytodo_html($variables) {
	module_load_include('inc', 'mytinytodo', 'mytinytodo/init');

	$delta = $variables['delta'];
	$eid = $variables['eid'];
	$output = '';

	$lang = Lang::instance();

	if ($lang->rtl()) 
		Config::set('rtl', 1);
	if (!is_int(Config::get('firstdayofweek')) || Config::get('firstdayofweek') < 0 || Config::get('firstdayofweek') > 6)
		Config::set('firstdayofweek', 1);

	if (!defined('TEMPLATEPATH'))
		define('TEMPLATEPATH', MTTPATH. 'themes/'.Config::get('template').'/');

	global $base_path;
	$path = $base_path . drupal_get_path('module', 'mytinytodo') . '/';
	mytinytodo_add_css($path . 'mytinytodo/themes/default/list-style.css', array('group' => CSS_DEFAULT));

	if (isset($variables['pda']) && $variables['pda'] === true) {
		$viewport = array(
				'#tag' => 'meta',
				'#attributes' => array(
					'name' => 'viewport',
					'id' => 'viewport',
					'content' => 'width=device-width',
					),
				);
		mytinytodo_add_css($path . 'mytinytodo/themes/default/pda.css', array('group' => CSS_DEFAULT));
	}

	mytinytodo_add_library('system','jquery');
	mytinytodo_add_library('system','jquery.once');
	mytinytodo_add_js($base_path  . 'misc/ui/jquery.ui.core.min.js');
	mytinytodo_add_js($base_path . 'misc/jquery.ba-bbq.js');
	mytinytodo_add_library('system','jquery.cookie');
	mytinytodo_add_library('system','ui.widget');
	mytinytodo_add_library('system','ui.mouse');
	mytinytodo_add_js($path . 'mytinytodo/jquery/jquery.autocomplete-1.1.js');
	mytinytodo_add_library('system','effects');
	mytinytodo_add_library('system','effects.highlight');
	mytinytodo_add_library('system','ui.datepicker');
	mytinytodo_add_library('system','ui.sortable');

	$jsoutput = '';
	if (is_numeric($delta))
		$jsoutput .= "var delta = {$delta};\n";
	if (is_numeric($eid))
		$jsoutput .= "var entity_id = {$eid};\n";

	$jsoutput .= "var base_url = '" . url(NULL, array('absolute' => TRUE)) . "';\n";

	drupal_add_js($jsoutput, 'inline');

	mytinytodo_add_js($path . 'mytinytodo/mytinytodo.js');
	mytinytodo_add_js($path . 'mytinytodo/mytinytodo_lang.php');
	mytinytodo_add_js($path . 'mytinytodo/mytinytodo_ajax_storage.js');
	mytinytodo_add_js($base_path . 'mytinytodo/init');

	$output .= '
	<div id="wrapper">
	<div id="container">
	<div id="mtt_body">

	<div id="loading"></div>

	<div id="bar">
	<div id="msg"><span class="msg-text"></span><div class="msg-details"></div></div>
	<div class="bar-menu">
	<span class="menu-owner" style="display:none">
	<a href="#settings" id="settings">' . _e('a_settings') . '</a>
	</span>
	</div>
	</div>

	<br clear="all" />

	<div id="page_tasks" style="display:none">

	<div id="lists">
	<ul class="mtt-tabs"></ul>
	<div class="mtt-tabs-add-button" title="' . _e('list_new') . '"><span></span></div>
	<div id="tabs_buttons">
	<div class="mtt-tabs-select-button mtt-tabs-button" title="' . _e('list_select') . '"><span></span></div>
	</div>
	<div id="list_all" class="mtt-tab mtt-tabs-alltasks mtt-tabs-hidden"><a href="#alltasks"><span>' . _e('alltasks') . '</span><div class="list-action"></div></a></div>
	</div>

	<div id="toolbar" class="mtt-htabs">

	<div id="htab_search">
	<table class="mtt-searchbox"><tr><td>
	<div class="mtt-searchbox-c">
	<input type="text" name="search" value="" maxlength="250" id="search" autocomplete="off" />
	<div class="mtt-searchbox-icon mtt-icon-search"></div>
	<div id="search_close" class="mtt-searchbox-icon mtt-icon-cancelsearch"></div>
	</div> 
	</td></tr></table>
	</div>

	<div id="htab_newtask">
	<table class="mtt-taskbox"><tr><td class="mtt-tb-cell">
	<div class="mtt-tb-c">
	<form id="newtask_form" method="post">
	<label id="task_placeholder" class="placeholding" for="task">
	 <input type="text" name="task" value="" maxlength="250" id="task" autocomplete="off" />
	 <span>' . _e('htab_newtask') . '</span>
	</label>
	<div id="newtask_submit" class="mtt-taskbox-icon mtt-icon-submittask" title="' . _e('btn_add') . '"></div>
	</form>
	</div>
	</td>
	<td><a href="#" id="newtask_adv" class="mtt-img-button" title="' . _e('advanced_add') . '"><span></span></a></td>
	</tr></table>
	</div>

	<div id="searchbar" style="display:none">' . _e('searching') . ' <span id="searchbarkeyword"></span></div>

	<div style="clear:both"></div>

	</div>


	<h3>
	<span id="taskview" class="mtt-menu-button"><span class="btnstr">' . _e('tasks') . '</span> (<span id="total">0</span>) <span class="arrdown"></span></span>
	<span class="mtt-notes-showhide">' . _e('notes') . ' <a href="#" id="mtt-notes-show">' . _e('notes_show') . '</a> / <a href="#" id="mtt-notes-hide">' . _e('notes_hide') . '</a></span>
	<span id="mtt_filters"></span>
	<span id="tagcloudbtn" class="mtt-menu-button">' . _e('tagcloud') . ' <span class="arrdown2"></span></span>
	</h3>

	<div id="taskcontainer">
	<ol id="tasklist" class="sortable"></ol>
	</div>

	</div> <!-- end of page_tasks -->


	<div id="page_taskedit" style="display:none">

	<div><a href="#" class="mtt-back-button">' . _e('go_back') . '</a></div>

	<h3 class="mtt-inadd">' . _e('add_task') . '</h3>
	<h3 class="mtt-inedit">' . _e('edit_task') . '
	<div id="taskedit-date" class="mtt-inedit">
	(<span class="date-created" title="' . _e('taskdate_created') . '"><span></span></span><span class="date-completed" title="' . _e('taskdate_completed') . '"> &mdash; <span></span></span>)
	</div>
	</h3>

	<form id="taskedit_form" name="edittask" method="post">
	<input type="hidden" name="isadd" value="0" />
	<input type="hidden" name="id" value="" />
	<div class="form-row form-row-short">
	<span class="h">' . _e('priority') . '</span>
	<select name="prio">
	<option value="2">+2</option><option value="1">+1</option><option value="0" selected="selected">&plusmn;0</option><option value="-1">&minus;1</option>
	</select>
	</div>
	<div class="form-row form-row-short">
	<span class="h">' . _e('due') . ' </span>
	<input name="duedate" id="duedate" value="" class="in100" title="Y-M-D, M/D/Y, D.M.Y, M/D, D.M" autocomplete="off" />
	</div>
	<div class="form-row-short-end"></div>
	<div class="form-row"><div class="h">' . _e('task') . '</div> <input type="text" name="task" value="" class="in500" maxlength="250" /></div>
	<div class="form-row"><div class="h">' . _e('note') . '</div> <textarea name="note" class="in500"></textarea></div>
	<div class="form-row"><div class="h">' . _e('tags') . '</div>
	<table cellspacing="0" cellpadding="0" width="100%"><tr>
	<td><input type="text" name="tags" id="edittags" value="" class="in500" maxlength="250" /></td>
	<td class="alltags-cell">
	<a href="#" id="alltags_show">' . _e('alltags_show') . '</a>
	<a href="#" id="alltags_hide" style="display:none">' . _e('alltags_hide') . '</a></td>
	</tr></table>
	</div>
	<div class="form-row" id="alltags" style="display:none;">' . _e('alltags') . ' <span class="tags-list"></span></div>
	<div class="form-row form-bottom-buttons">
	<input type="submit" value="' . _e('save') . '" /> 
	<input type="button" id="mtt_edit_cancel" class="mtt-back-button" value="' . _e('cancel') . '" />
	</div>
	</form>

	</div>  <!-- end of page_taskedit -->


	<div id="priopopup" style="display:none">
	<span class="prio-neg prio-neg-1">&minus;1</span>
	<span class="prio-zero">&plusmn;0</span>
	<span class="prio-pos prio-pos-1">+1</span>
	<span class="prio-pos prio-pos-2">+2</span>
	</div>

	<div id="taskviewcontainer" class="mtt-menu-container" style="display:none">
	<ul>
	<li id="view_tasks">' . _e('tasks') . ' (<span id="cnt_total">0</span>)</li>
	<li id="view_past">' . _e('f_past') . ' (<span id="cnt_past">0</span>)</li>
	<li id="view_today">' . _e('f_today') . ' (<span id="cnt_today">0</span>)</li>
	<li id="view_soon">' . _e('f_soon') . ' (<span id="cnt_soon">0</span>)</li>
	</ul>
	</div>

	<div id="tagcloud" style="display:none">
	<a id="tagcloudcancel" class="mtt-img-button"><span></span></a>
	<div id="tagcloudload"></div>
	<div id="tagcloudcontent"></div>
	</div>


	<div id="listmenucontainer" class="mtt-menu-container" style="display:none">
	<ul>
	<li class="mtt-need-list mtt-need-real-list" id="btnRenameList">' . _e('list_rename') . '</li>
	<li class="mtt-need-list mtt-need-real-list" id="btnDeleteList">' . _e('list_delete') . '</li>
	<li class="mtt-need-list mtt-need-real-list" id="btnClearCompleted">' . _e('list_clearcompleted') . '</li>
	<li class="mtt-need-list mtt-need-real-list mtt-menu-indicator" submenu="listexportmenucontainer"><div class="submenu-icon"></div>' . _e('list_export') . '></li>
	<li class="mtt-menu-delimiter mtt-need-real-list"></li>
	<li class="mtt-need-list mtt-need-real-list" id="btnPublish"><div class="menu-icon"></div>' . _e('list_publish') . '</li>
	<li class="mtt-need-list mtt-need-real-list" id="btnRssFeed"><div class="menu-icon"></div>' . _e('list_rssfeed') . '</li>
	<li class="mtt-menu-delimiter mtt-need-real-list"></li>
	<li class="mtt-need-list mtt-need-real-list sort-item" id="sortByHand"><div class="menu-icon"></div>' . _e('sortByHand') . ' <span class="mtt-sort-direction"></span></li>
	<li class="mtt-need-list sort-item" id="sortByDateCreated"><div class="menu-icon"></div>' . _e('sortByDateCreated') . ' <span class="mtt-sort-direction"></span></li>
	<li class="mtt-need-list sort-item" id="sortByPrio"><div class="menu-icon"></div>' . _e('sortByPriority') . ' <span class="mtt-sort-direction"></span></li>
	<li class="mtt-need-list sort-item" id="sortByDueDate"><div class="menu-icon"></div>' . _e('sortByDueDate') . ' <span class="mtt-sort-direction"></span></li>
	<li class="mtt-need-list sort-item" id="sortByDateModified"><div class="menu-icon"></div>' . _e('sortByDateModified') . ' <span class="mtt-sort-direction"></span></li>
	<li class="mtt-menu-delimiter"></li>
	<li class="mtt-need-list" id="btnShowCompleted"><div class="menu-icon"></div>' . _e('list_showcompleted') . '</li>
	</ul>
	</div>

	<div id="listexportmenucontainer" class="mtt-menu-container" style="display:none">
	<ul>
	<li class="mtt-need-list mtt-need-real-list" id="btnExportCSV">' . _e('list_export_csv') . '</li>
	<li class="mtt-need-list mtt-need-real-list" id="btnExportICAL">' . _e('list_export_ical') . '</li>
	</ul>
	</div>

	<div id="taskcontextcontainer" class="mtt-menu-container" style="display:none">
	<ul>
	<li id="cmenu_edit"><b>' . _e('action_edit') . '</b></li>
	<li id="cmenu_note">' . _e('action_note') . '</li>
	<li id="cmenu_prio" class="mtt-menu-indicator" submenu="cmenupriocontainer"><div class="submenu-icon"></div>' . _e('action_priority') . '</li>
	<li id="cmenu_move" class="mtt-menu-indicator" submenu="cmenulistscontainer"><div class="submenu-icon"></div>' . _e('action_move') . '</li>
	<li id="cmenu_delete">' . _e('action_delete') . '</li>
	</ul>
	</div>

	<div id="cmenupriocontainer" class="mtt-menu-container" style="display:none">
	<ul>
	<li id="cmenu_prio:2"><div class="menu-icon"></div>+2</li>
	<li id="cmenu_prio:1"><div class="menu-icon"></div>+1</li>
	<li id="cmenu_prio:0"><div class="menu-icon"></div>&plusmn;0</li>
	<li id="cmenu_prio:-1"><div class="menu-icon"></div>&minus;1</li>
	</ul>
	</div>

	<div id="cmenulistscontainer" class="mtt-menu-container" style="display:none">
	<ul>
	</ul>
	</div>

	<div id="slmenucontainer" class="mtt-menu-container" style="display:none">
	<ul>';

	if (is_readonly())
		$output .= '<li id="slmenu_list:-1" class="list-id--1 mtt-need-list" style="display:none">';
	else
		$output .= '<li id="slmenu_list:-1" class="list-id--1 mtt-need-list">';

	$output .= '<div class="menu-icon"></div><a href="#alltasks">' . _e('alltasks') . '></a></li>';

	if (is_readonly())
		$output .= '<li class="mtt-menu-delimiter slmenu-lists-begin mtt-need-list" style="display:none"></li>';
	else
		$output .= '<li class="mtt-menu-delimiter slmenu-lists-begin mtt-need-list"></li>';

	$output .='
	</ul>
	</div>

	<div id="page_ajax" style="display:none"></div>

	</div>
	<div id="space"></div>
	</div>

	</div>';

	return $output;
}
