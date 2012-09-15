/*
	This file is a part of myTinyTodo.
	(C) Copyright 2010 Max Pozdeev <maxpozdeev@gmail.com>
	Licensed under the GNU GPL v3 license. See file COPYRIGHT for details.
*/

// AJAX myTinyTodo Storage

(function($){

var mtt;
var base_path = base_url ? base_url : '/';

function mytinytodoStorageAjax(amtt) 
{
	this.mtt = mtt = amtt;
}

window.mytinytodoStorageAjax = mytinytodoStorageAjax;

mytinytodoStorageAjax.prototype = 
{
	/* required method */
	request:function(action, params, callback)
	{
		if (!this[action]) 
			throw "Unknown storage action: " + action;

		this[action](params, function(json){
			if (json.denied)
				mtt.errorDenied();
			if (callback)
				callback.call(mtt, json)
		});
	},


	loadLists: function(params, callback)
	{
		$.getJSON(base_path + 'mytinytodo/ajax?loadLists'+'&rnd='+Math.random() + '&delta=' + delta + '&eid=' + entity_id, callback);
	},


	loadTasks: function(params, callback)
	{
		var q = '';
		if(params.search && params.search != '') q += '&s='+encodeURIComponent(params.search);
		if(params.tag && params.tag != '') q += '&t='+encodeURIComponent(params.tag);
		if(params.setCompl && params.setCompl != 0) q += '&setCompl=1';
		q += '&rnd='+Math.random();

		$.getJSON(base_path + 'mytinytodo/ajax?loadTasks&list='+params.list+'&compl='+params.compl+'&sort='+params.sort+q + '&delta=' + delta + '&eid=' + entity_id, callback);
	},


	newTask: function(params, callback)
	{
		$.post(base_path + 'mytinytodo/ajax?newTask',
			{ list:params.list, title: params.title, tag:params.tag, delta: delta, eid: entity_id }, callback, 'json');
	},
	

	fullNewTask: function(params, callback)
	{
		$.post(base_path + 'mytinytodo/ajax?fullNewTask',
			{ list:params.list, title:params.title, note:params.note, prio:params.prio, tags:params.tags, duedate:params.duedate, delta: delta, eid: entity_id },
			callback, 'json');
	},


	editTask: function(params, callback)
	{
		$.post(base_path + 'mytinytodo/ajax?editTask='+params.id,
			{ id:params.id, title:params.title, note:params.note, prio:params.prio, tags:params.tags, duedate:params.duedate, delta: delta, eid: entity_id },
			callback, 'json');
	},


	editNote: function(params, callback)
	{
		$.post(base_path + 'mytinytodo/ajax?editNote='+params.id, {id:params.id, note: params.note, delta: delta, eid: entity_id}, callback, 'json');
	},


	completeTask: function(params, callback)
	{
		$.post(base_path + 'mytinytodo/ajax?completeTask='+params.id, { id:params.id, compl:params.compl, delta: delta, eid: entity_id }, callback, 'json');
	},


	deleteTask: function(params, callback)
	{
		$.post(base_path + 'mytinytodo/ajax?deleteTask='+params.id, { id:params.id, delta: delta, eid: entity_id }, callback, 'json');
	},


	setPrio: function(params, callback)
	{
		$.getJSON(base_path + 'mytinytodo/ajax?setPrio='+params.id+'&prio='+params.prio+'&rnd='+Math.random() + '&delta=' + delta + '&eid=' + entity_id, callback);
	},

	
	setSort: function(params, callback)
	{
		$.post(base_path + 'mytinytodo/ajax?setSort', { list:params.list, sort:params.sort, delta: delta, eid: entity_id }, callback, 'json');
	},

	changeOrder: function(params, callback)
	{
		var order = '';
		for(var i in params.order) {
			order += params.order[i].id +'='+ params.order[i].diff + '&';
		}
		$.post(base_path + 'mytinytodo/ajax?changeOrder', { order:order, delta: delta, eid: entity_id }, callback, 'json');
	},

	tagCloud: function(params, callback)
	{
		$.getJSON(base_path + 'mytinytodo/ajax?tagCloud&list='+params.list+'&rnd='+Math.random() + '&delta=' + delta + '&eid=' + entity_id, callback);
	},

	moveTask: function(params, callback)
	{
		$.post(base_path + 'mytinytodo/ajax?moveTask', { id:params.id, from:params.from, to:params.to, delta: delta, eid: entity_id }, callback, 'json');
	},

	parseTaskStr: function(params, callback)
	{
		$.post(base_path + 'mytinytodo/ajax?parseTaskStr', { list:params.list, title:params.title, tag:params.tag, delta: delta, eid: entity_id }, callback, 'json');
	},
	

	// Lists
	addList: function(params, callback)
	{
		$.post(base_path + 'mytinytodo/ajax?addList', { name:params.name, delta: delta, eid: entity_id }, callback, 'json'); 

	},

	renameList:  function(params, callback)
	{
		$.post(base_path + 'mytinytodo/ajax?renameList', { list:params.list, name:params.name, delta: delta, eid: entity_id }, callback, 'json');
	},

	deleteList: function(params, callback)
	{
		$.post(base_path + 'mytinytodo/ajax?deleteList', { list:params.list, delta: delta, eid: entity_id }, callback, 'json');
	},

	publishList: function(params, callback)
	{
		$.post(base_path + 'mytinytodo/ajax?publishList', { list:params.list, publish:params.publish, delta: delta, eid: entity_id },  callback, 'json');
	},
	
	setShowNotesInList: function(params, callback)
	{
	    $.post(base_path + 'mytinytodo/ajax?setShowNotesInList', { list:params.list, shownotes:params.shownotes, delta: delta, eid: entity_id },  callback, 'json');
	},
	
	setHideList: function(params, callback)
	{
		$.post(base_path + 'mytinytodo/ajax?setHideList', { list:params.list, hide:params.hide, delta: delta, eid: entity_id }, callback, 'json');
	},

	changeListOrder: function(params, callback)
	{
		$.post(base_path + 'mytinytodo/ajax?changeListOrder', { order:params.order, delta: delta, eid: entity_id }, callback, 'json');
	},

	clearCompletedInList: function(params, callback)
	{
		$.post(base_path + 'mytinytodo/ajax?clearCompletedInList', { list:params.list, delta: delta, eid: entity_id }, callback, 'json');
	}

};

})(jQuery);
