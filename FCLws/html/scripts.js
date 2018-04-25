
var Event = function (sender) {
    this._sender = sender;
    this._listeners = [];
}

Event.prototype = {

    attach: function (listener) {
        this._listeners.push(listener);
    },

    notify: function (...args) {
        for (var i = 0; i < this._listeners.length; i += 1) {
            this._listeners[i](...args);
        }
    }
};

function FCLModel()
{
   this.blueprints = {};
   this.connections = {};
   this.nodes = {};
   this.addBlueprintEvent = new Event(this);
   this.removeBlueprintEvent = new Event(this);
   this.addNodeEvent = new Event(this);
   this.deleteNodeEvent = new Event(this);
   this.addConnectionEvent = new Event(this);
   this.valueRecalculatedEvent = new Event(this);
   this.removeConnectionEvent = new Event(this);
}

FCLModel.prototype = 
{
   addBlueprint: function(blueprint)
   {
      this.blueprints[blueprint.id] = blueprint;
      this.addBlueprintEvent.notify(blueprint);
   },

   removeBlueprint: function(blueprint_id)
   {
      if (blueprint_id in this.blueprints)
      {
         delete this.blueprints[blueprint_id];
         return;
      }
      console.log("Attempt to delete blueprint with id ", blueprint.id, " failed.");
   },

   updateSourceNode : function(node_id, value)
   {
      if (node_id in this.nodes && this.nodes[node_id].type=="source")
      {
         this.nodes[node_id].value = value;
         return;
      }
      console.log("Unable to update source with id", node_id, "Reason : not found");
   },

   updateSinkNode : function(node_id, value)
   {
      if (node_id in this.nodes && this.nodes[node_id].type=="sink")
      {
         this.nodes[node_id].value = value;
         return;
      }
      console.log("Unable to update sink with id", node_id, "Reason : not found");
   },

   addNode: function(node)
   {
      this.nodes[node.id] = node;
      this.addNodeEvent.notify(node);
   },

   deleteNode: function(node_id)
   {
      if (node_id in this.nodes)
      {
         delete this.nodes[node_id];
         return;
      }
      console.log("Attempt to delete node with id ", node.id, " failed.");
   },

   addConnection: function(connection)
   {
      this.connections[connection.target] = connection;
      this.addConnectionEvent.notify(connection);
   },

   removeConnection: function(connection)
   {
      if (connection.target in this.connections)
      {
         delete this.connections[connection.target];
         this.removeConnectionEvent.notify(connection);
         return;
      }
      console.log("Attempt to delete connection ", connection_id, " failed.");
   }
}

function FCLController(model, view, ws)
{
   this.is_waiting_for_result = 0;
   this.view = view;
   this.model = model;
   this.ws = ws;
   this.node_id = 0;
   this.endpoint_id = 0;
}

FCLController.prototype =
{
   init: function()
   {
      this.view.newConnectionEvent.attach(
         this.newConnectionHandler.bind(this));

      this.view.nodeAddEvent.attach(
         this.newNodeHandler.bind(this));

      this.view.connectionDropEvent.attach(
         this.connectionDropHandler.bind(this));

      this.view.connectionDropEvent.attach(
         this.connectionDropHandler.bind(this));

      this.view.valueUpdatedEvent.attach(
          this.valueUpdatedHandler.bind(this));

      var model = this.model;
      var ws = this.ws;
      var view = this.view;
     ws.onopen = function()
     {
       ws.send(JSON.stringify({type : 'hello'}));
     };

     ws.onmessage = function (evt) 
     { 
       var received_msg = evt.data;
       var msg = JSON.parse(received_msg);
       console.log(msg);
       if (msg.type == "hi")
       {
        var bp_id = 1;
        var serializers = msg.content.serializers;
        var functions = msg.content.functions;
        for (var i = 0; i < serializers.length; i++)
        {
          var s = serializers[i];
          model.addBlueprint(
            {
              id : "blueprint_" + (bp_id++).toString(),
              name : s.type + "_value",
              type : "source",
              inputs : [],
              outputs : [ {type : s.type} ]
            });

          model.addBlueprint(
            {
              id : "blueprint_" + (bp_id++).toString(),
              name : s.type + "_result",
              type : "sink",
              inputs : [ {type : s.type} ],
              outputs : []
            });
        }

        for (var i = 0; i < functions.length; i++)
        {
          var f = functions[i];
          model.addBlueprint(
            {
              id : "blueprint_" + (bp_id++).toString(),
              name : f.name,
              type : "func",
              inputs : f.inputs,
              outputs : f.outputs
            });
        }

       }
       if (msg.type == "result")
       {
         for (var i = 0; i < msg.content.length; i++)
         {
           var el = msg.content[i];
           var node_id = el.node_name;
           var node_index = el.node_index;
           var result = el.result;
           var node = model.nodes[node_id];
           var source_endpoint_id = node.outputs[node_index].source_id;
           for (connection_id in model.connections)
           {
              var connection = model.connections[connection_id];
              if (source_endpoint_id == connection.source)
              {
                 var target_endpoint_id = connection.target;
                 for (sink_node_id in model.nodes)
                 {
                   var sink_node = model.nodes[sink_node_id];
                   if (sink_node.type == "sink" && 
                       sink_node.inputs[0].target_id == target_endpoint_id)
                   {
                     model.valueRecalculatedEvent.notify(sink_node_id, result);
                   }
                 }
              }
           }

         }
       }
     };
   },

   constructProgram: function(model)
   {
      if (this.is_waiting_for_result == 1)
      {
         return;
      }

      var endpoint_to_target_node = {}
      var endpoint_to_source_node = {}
      var unresolved_endpoints = {}
      var program = "";
      var queue = [];

      for (node_id in model.nodes)
      {
         var node = model.nodes[node_id]
         unresolved_endpoints[node.id] = node.inputs.length;
         if (unresolved_endpoints[node.id] == 0)
         {
            queue.push(node.id);
         }

         for (var i = 0; i < node.inputs.length; i++)
         {
            var input = node.inputs[i];
            endpoint_to_target_node[input.target_id] =  node.id;
         }
      }


      var result = "";
      while (queue.length != 0)
      {
         var node_id = queue.pop();
         var node = model.nodes[node_id];
         if (node.type == "func")
         {
            var bp = model.blueprints[node.blueprint_id];
            result += node.id + " = " + bp.name + "(";
            for (var i = 0; i < node.inputs.length; i++)
            {
               var input = node.inputs[i];
               var tmp = endpoint_to_source_node[input.target_id];
               var source_node = model.nodes[tmp.node_id];
               result += source_node.id + "." + tmp.output;
               if (i != node.inputs.length - 1)
               {
                  result += ",";
               }
            }
            result += ")\n";
         }
         if (node.type == "source")
         {
            result += node.outputs[0].type + " " + node.id + " = " + "'" + node.value + "'" + '\n';
         }

         if (node.type == "sink")
         {
            var input = node.inputs[0];
            var tmp = endpoint_to_source_node[input.target_id];
            var source_node = model.nodes[tmp.node_id];
            result += "return " + source_node.id + "." + tmp.output + '\n';
         }
         for (connection_id in model.connections)
         {
            var connection = model.connections[connection_id];
            for (var i = 0; i < node.outputs.length; i++)
            {
               var output = node.outputs[i];
               if (connection.source == output.source_id)
               {
                  var other_node_id = endpoint_to_target_node[connection.target];
                  endpoint_to_source_node[connection.target] = {node_id : node_id, output : i};
                  unresolved_endpoints[other_node_id]--;
                  if (unresolved_endpoints[other_node_id] == 0)
                  {
                    queue.push(other_node_id);
                  }
               }
            }
         }
      }

      this.ws.send(JSON.stringify({type : 'evaluate', program : result}));
      console.log(result);

   },

   valueUpdatedHandler: function (node_id, value)
   {
      this.model.updateSourceNode(node_id, value);
      console.log(this.model.connections, this.model.nodes);
      this.constructProgram(this.model);
   },

   newConnectionHandler: function (connection)
   {
      for (var i = 0; i < this.model.connections.length; i++)
      {
         if (this.model.connections[i].target == connection.target)
         {
            this.model.removeConnection(connections[i]);               
         }
      }
      this.model.addConnection(connection);
   },

   connectionDropHandler: function (target_id)
   {
      for (var i = 0; i < this.model.connections.length; i++)
      {
         if (this.model.connections[i].target == target_id)
         {
            this.model.removeConnection(this.model.connections[i]);           
         }
      }
   },

   newNodeHandler: function(blueprint_id)
   {
      if (!blueprint_id in this.model.blueprints)
      {
         console.log("Bluprint", blueprint_id, "not found");
         return;
      }

      var blueprint = this.model.blueprints[blueprint_id];
      var id = (this.node_id++).toString();
      var node =
      {
         id: "node_" + id,
         name: blueprint.name + id,
         type: blueprint.type,
         value: "",
         blueprint_id: blueprint.id,
         inputs: [],
         outputs: []
      }

      for (var i = 0; i < blueprint.inputs.length; i++)
      {
         var e_id = (this.endpoint_id++).toString();
         var itype = blueprint.inputs[i].type;
         node.inputs.push({ type: itype, target_id: "endpoint_" + e_id});
      }

      for (var i = 0; i < blueprint.outputs.length; i++)
      {
         var e_id = (this.endpoint_id++).toString();
         var otype = blueprint.outputs[i].type;
         node.outputs.push({ type: otype, source_id: "endpoint_" + e_id});
      }

      this.model.addNode(node);
   }
}

function FCLView(model, widgetFactory)
{
   this.model = model;
   this.nodeAddEvent = new Event(this);
   this.nodeRemoveEvent = new Event(this);
   this.newConnectionEvent = new Event(this);
   this.connectionDropEvent = new Event(this);
   this.valueUpdatedEvent = new Event(this);
   this.widgetFactory = widgetFactory;
   this.nodeUpdateFunctions = []
}

FCLView.prototype =
{
   init: function()
   {
      this.graph = new Graph('canvas', 'connections');
      this.blueprint_list = new BlueprintList('sidebar');

      this.model.addConnectionEvent.attach(
         this.addConnectionEventHandler.bind(this));

      this.model.removeConnectionEvent.attach(
         this.removeConnectionEventHandler.bind(this));

      this.model.addBlueprintEvent.attach(
         this.addBlueprintEventHandler.bind(this));

      this.model.removeBlueprintEvent.attach(
         this.removeBlueprintEventHandler.bind(this));

      this.model.addNodeEvent.attach(
         this.addNodeEventHandler.bind(this));

      this.model.valueRecalculatedEvent.attach(
         this.valueRecalculatedEventHandler.bind(this));

      var graph = this.graph;
      var newConnEvent = this.newConnectionEvent;
      var connDropEvent = this.connectionDropEvent;
      var nodeAddEvent = this.nodeAddEvent;

      $('#canvas').droppable(
      {
         drop: function( event, ui ) {
            var id = ui.draggable.attr('id');
            if (id.startsWith("blueprint"))
            {
               nodeAddEvent.notify(id)
               console.log(id);
            }
         }
      });
   },

   addConnectionEventHandler: function(conn)
   {
      $(".target_endpoint[id" + "=" + conn.target + "]")
         .css('background-color', '#FF9900');
      this.graph.connections.add(conn);
      this.graph.draw();
   },

   removeConnectionEventHandler: function(conn)
   {
      $(".target_endpoint[id" + "=" + conn.target + "]")
         .css('background-color', '#BCBCBC');
      this.graph.connections.drop_if(selector_equal(conn));
      this.graph.draw();
   },

   addBlueprintEventHandler: function(blueprint)
   {
      this.blueprint_list.addBlueprint(blueprint);
   },

   removeBlueprintEventHandler: function(blueprint)
   {
      this.blueprint_list.removeBlueprint(blueprint.id);
   },

   valueRecalculatedEventHandler: function(node_id, data)
   {
      for (var i = 0; i < this.nodeUpdateFunctions.length; i++)
      {
         nuf = this.nodeUpdateFunctions[i];
         if (nuf.node_id == node_id)
         {
            nuf.function(data);
            return;
         }
      }
      console.log("Failed to update value of node with node id=", node_id);
   },

   addNodeEventHandler: function(node_data)
   {
      var graph = this.graph;
      var newConnEvent = this.newConnectionEvent;
      var connDropEvent = this.connectionDropEvent;

      var node = $('<div class="node" id="' + node_data.id + '"></div>');
      var title = $('<div class="node_title"></div>').text(node_data.name);

      node.append(title);

      if (node_data.type == "source")
      {
         var widget = this.widgetFactory.getSourceWidget(
              node_data.id, 
              node_data.outputs[0].type, 
              this.valueUpdatedEvent.notify.bind(this.valueUpdatedEvent));

         
         var arg = $('<div class="argument"></div>');
         arg.append(widget);
         node.append(arg);
      }

      if (node_data.type == "sink")
      {
         var widget = this.widgetFactory.getSinkWidget(node_data.id, node_data.inputs[0].type);
         
         var ret = $('<div class="return"></div>');
         ret.append(widget.dom);
         node.append(ret);

         this.nodeUpdateFunctions.push({ node_id: node_data.id, function: widget.updateFunction });
      }

      for (var i = node_data.inputs.length - 1; i >= 0; i--) {
         var arg = $('<div class="argument"></div>');
         var name = $('<div class="arg_name"></div>').text(node_data.inputs[i].type);
         var endpoint = $('<div class="target_endpoint"></div>').attr('id', node_data.inputs[i].target_id);
         endpoint.droppable(
         {
            drop: function( event, ui ) {
               var id = $(this).attr('id');
               newConnEvent.notify(
                  {source: ui.draggable.attr('id'), target: id});
            }
         });
         endpoint.click(function(){
            var id = $(this).attr('id');
            connDropEvent.notify(id);
         })
         arg.append(name, endpoint);
         node.append(arg);
      }

      for (var i = node_data.outputs.length - 1; i >= 0; i--) {
         var ret = $('<div class="return"></div>');
         var name = $('<div class="ret_name"></div>').text(node_data.outputs[i].type);
         var endpoint = $('<div class="source_endpoint"></div>').attr('id', node_data.outputs[i].source_id);
         endpoint.draggable({ helper: "clone" });
         ret.append(name, endpoint);
         node.append(ret);
      }

      node.draggable({drag: function(){ graph.draw() }});
      $("#canvas").append(node);
   }
}

function BlueprintList(div_id)
{
   this.main_div = $('#' + div_id);
   this.list_div = $('<ul></ul>');
   this.main_div.append(this.list_div);
}

BlueprintList.prototype = 
{
   addBlueprint: function(blueprint)
   {
      var list_entry = $('<li id="' + blueprint.id + '"></li>');
      var bp_title = $('<div class=node_title></div>').text(blueprint.name);
      list_entry.append(bp_title);
      list_entry.draggable({ helper: "clone" });

      this.list_div.append(list_entry);
   },

   removeBlueprint: function(blueprint_id)
   {
      var bp = $("li[id=" + blueprint_id + "]");
      bp.remove();
   }
}

function WidgetFactory(
  defaultSourceCreator,
  defaultSinkCreator,
  widgetCreators = [])
{
  this.widgetCreators = widgetCreators;
  this.defaultSourceCreator = defaultSourceCreator;
  this.defaultSinkCreator = defaultSinkCreator;
}

WidgetFactory.prototype =
{
  addWidget : function(widgetCreator)
  {
    widgetCreators.push(widgetCreator);
  },

  getSinkWidget : function(node_id, type)
  {
      var wc = this.findWidget(type, "sink");
      if (wc == null)
      {
         return this.defaultSinkCreator(node_id);
      }

      return wc.function(node_id);
  },

  getSourceWidget : function(node_id, type, onUpdate)
  {
      var wc = this.findWidget(type, "source");
      if (wc == null)
      {
          return this.defaultSourceCreator(node_id, onUpdate);
      }

      return wc.function(node_id, onUpdate);
  },

  findWidget : function(type, widgetType)
  {
     for (var i = 0; i < this.widgetCreators.length; i++)
     {
        var wc = widgetCreators[i];
        if (wc.type = type, wc.widgetType = widgetType)
        {
           return wc;
        }
     }
     return null;
  }
}

function Connections()
{
  var connections = [];
  var on_drop_callbacks = [];
  var on_add_callbacks = [];
  var canvas = document.getElementById('connections');
  var ctx = canvas.getContext("2d");
  function notify_on_add(source, target)
  {
    for (var i = 0; i < on_add_callbacks.length; i++)
    {
      on_add_callbacks[i](source, target);
    }
  };
  function notify_on_drop(source, target)
  {
    for (var i = 0; i < on_add_callbacks.length; i++)
    {
      on_drop_callbacks[i](source, target);
    }
  };
  this.add = function(c)
  {
    this.drop_if(function(conn){ return conn.target === c.target;});
    connections.push(c);
    notify_on_add(c.source, c.target);
  };
  this.drop_if = function(selector)
  {
    for (var i = 0; i < connections.length; i++)
    {
      if (selector(connections[i]))
      {
        var conn = connections[i];
        connections.splice(i, 1);
        i--;
        notify_on_drop(conn.source, conn.target);                  
      }
    }
  };
  this.on_drop = function(callback)
  {
    on_drop_callbacks.push(callback);
  };
  this.on_add = function(callback)
  {
    on_add_callbacks.push(callback);
  };
  this.for_each = function(functor)
  {
    for (var i = 0; i < connections.length; i++)
    {
      functor(connections[i]);
    }
  }
}

function GraphCanvas(id)
{
  var canvas = document.getElementById(id);
  var ctx = canvas.getContext('2d');
  this.clear = function()
  {
    ctx.clearRect(0, 0, canvas.width, canvas.height);
  }
  this.setDimensions = function(width, height)
  {
    canvas.width = width;
    canvas.height = height;
  }
  this.draw_line = function(p1, p2, color)
  {
    ctx.beginPath();
    ctx.moveTo(p1.x, p1.y);
    ctx.lineTo(p2.x, p2.y);
    ctx.closePath();
    ctx.lineWidth = 2;
    ctx.strokeStyle = color;
    ctx.stroke();
  }
}

function Graph(graph_div_id, canvas_id)
{
  var self = this;
  var graph = $('#' + graph_div_id);
  var canvas = new GraphCanvas(canvas_id);
  this.connections = new Connections();
  this.get_position = function(obj)
  {
    var wrapper = graph.offset();
    var borderLeft =  parseInt(graph.css("border-left-width"), 10);
    var borderTop = parseInt(graph.css("border-top-width"), 10);
    var pos = obj.offset();
    var left = pos.left - wrapper.left - borderLeft;
    var top = pos.top - wrapper.top - borderTop;
    return { x: left, y: top }
  }
  this.draw = function()
  {
    canvas.clear();
    this.connections.for_each(function(conn)
    {
      var source = $('#' + conn.source);
      var target = $('#' + conn.target);
      var source_pos = self.get_position(source);
      var target_pos = self.get_position(target);
      source_pos.x += source.width()/2;
      source_pos.y += source.height()/2;
      target_pos.x += target.width()/2;
      target_pos.y += target.height()/2;
      canvas.draw_line(source_pos, target_pos, '#FF9900');
    });
  }
  this.connections.on_drop(function(){ self.draw(); });
  this.connections.on_add(function(){ self.draw(); });
  canvas.setDimensions(graph.width(), graph.height());
  window.onresize = function() {
    canvas.setDimensions(graph.width(), graph.height());
    self.draw();
  }
}

function selector_hasTarget(target)
{
  return function (conn) { return target === conn.target; };
}

function selector_equal(connection)
{
   return function (conn) { return _.isEqual(conn, connection); };
}

$(function() {
   var widgetFactory = new WidgetFactory(function(node_id, onUpdate)
   {
      var widget = $('<input type="text" name="xd">');
      widget.on('input', function(e)
      {
         onUpdate(node_id, $(this).val());
      })
      return widget;
   },
   function()
   {
      var widget = $('<label> xdsadsa da</label>')
      return { dom: widget, updateFunction: function(value)
        {
          widget.html(value);
        }}
   })
   var model = new FCLModel();
   var view = new FCLView(model, widgetFactory);
   var ws = new WebSocket("ws://localhost:8080/");
   var controller = new FCLController(model, view, ws);
   controller.init();
   view.init();

   ws.onclose = function()
   { 
     // websocket is closed. 
   };

});

