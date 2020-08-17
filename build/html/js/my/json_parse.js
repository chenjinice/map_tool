



// 添加车道的折线
function add_lane_line(latlngs,id){
    var len = latlngs.length ,i,angle = 0;
    var line_color = "#FF0000";
    if(len == 0 )return;
    for(i=0;i<len;i++){
        if(i<len-1)angle = Math.atan2(latlngs[i+1][1]-latlngs[i][1],latlngs[i+1][0]-latlngs[i][0])*180/Math.PI;
        add_node_point(latlngs[i],angle);
    }
    if(len>1){
        var polyline = L.polyline(latlngs, {color:line_color,weight:5}).addTo(g_map);
        polyline.bindPopup("laneID="+id);
        polyline.source = "map";
        polyline.id = g_index++;
        g_layers.push(polyline);
    }
}


// 添加车道上点的图标
function add_node_point(latlng,angle)
{
    var marker = L.marker([latlng[0],latlng[1]],{icon:g_arrow_icon,rotationAngle:angle,rotationOrigin:'center',}).addTo(g_map);
    marker.source = "map";
    var lat_offset = latlng[0]*1e7 - g_lat;
    var lng_offset = latlng[1]*1e7 - g_lng;
    var tmp_angle = angle;
    if(tmp_angle <0)tmp_angle += 360.0;
    tmp_angle = tmp_angle.toFixed(2);
    marker.bindPopup("lng : "+latlng[1]+"</br>lat : "+latlng[0] +"</br>lng_offset : "+lng_offset+"</br>lat_offset : "+lat_offset +"</br>angle :"+tmp_angle);
//    marker.bindPopup("lng : "+latlng[1]+"</br>lat : "+latlng[0]);
    marker.id = g_index++;
    g_layers.push(marker);
}


// 添加node中心点图标
function add_center_point(lat,lng,n_id,n_region){
    var marker = L.marker([lat,lng]).addTo(g_map);
    marker.bindPopup("id : "+n_id+"</br>region : "+n_region+"</br>lng : "+lng+"</br>lat : "+lat);
    marker.source = "map";
    marker.id = g_index++;
    g_layers.push(marker);
}

// 解析node中的links
function json_links_parse(links)
{
    for(var i in links) {
        var lanes = links[i].lanes;
        for(var j in lanes){
            var points = lanes[j].points;
            var latlngs = [],latlngs_offset = [];
            for(var k in points){
                var p = points[k];
                var plng = (p.lng*1e-7).toFixed(7);
                var plat = (p.lat*1e-7).toFixed(7);
                latlngs.push([plat,plng]);
            }
            add_lane_line(latlngs,lanes[j].laneID);
        }
    }
}

// map
function add_map_json(data)
{
    var nodes = data.nodes;
    for(var i in nodes){
        var node = nodes[i];
        g_lng = node.refPos.lng;
        g_lat = node.refPos.lat;
        var lng = (g_lng*1e-7).toFixed(7);
        var lat = (g_lat*1e-7).toFixed(7);
        var node_id = node.id.id;
        var node_region = node.id.region;
        if("undefined" == typeof node_region)node_region="--";
        var inlinks = node.links;
        var center = [lat,lng];
        add_center_point(lat,lng,node_id,node_region);
        g_map.panTo(center,{animate:true,duration:0.5});
        json_links_parse(inlinks);
    }
}

// rsi
function add_rsi_json(data)
{
    g_lng = data.refPos.lng;
    g_lat = data.refPos.lat;
    var lng = (g_lng*1e-7).toFixed(7);
    var lat = (g_lat*1e-7).toFixed(7);
    var center = [lat,lng];
    g_map.panTo(center,{animate:true,duration:0.5});
    add_center_point(lat,lng,"--","--");
    var latlngs = [];
    for(var k in data.alertPath){
        var p = data.alertPath[k];
        var plng = (p.lng*1e-7).toFixed(7);
        var plat = (p.lat*1e-7).toFixed(7);
        latlngs.push([plat,plng]);
    }
    add_lane_line(latlngs,"--");
}

// 解析json文件
function json_parse(filename)
{
	$.getJSON(filename,function(data){
        var type = "map";
        if(data.type)type = data.type;
        if(type == "map")add_map_json(data);
        if(type == "rsi")add_rsi_json(data);
	}).error(function() {alert("parse json file error : "+filename);});
}






// ------------------------- geo json -----------------------------------
function add_geo_lane_line(latlngs,id){
    var len = latlngs.length ,i,angle = 0;
    var line_color = "#FF0000";
    if(len == 0 )return;
    for(i=0;i<len;i++){
        if(i<len-1)angle = Math.atan2(latlngs[i+1][1]-latlngs[i][1],latlngs[i+1][0]-latlngs[i][0])*180/Math.PI;
        var marker = L.marker([latlngs[i][0],latlngs[i][1]],{icon:g_arrow_icon,rotationAngle:angle,rotationOrigin:'center',});
        marker.bindPopup("lng : "+latlngs[i][1]+"</br>lat : "+latlngs[i][0]);
        g_group.addLayer(marker);
    }
    if(len>1){
        var polyline = L.polyline(latlngs, {color:line_color,weight:5}).addTo(g_map);
        polyline.bindPopup("laneID="+id);
        polyline.source = "map";
        polyline.id = g_index++;
        g_layers.push(polyline);
    }
}

//
function add_geo_rsu(point,id,region)
{
	add_center_point(point.lat,point.lng,id,region);
	for(i=5;i>0;i--){
		var m = 100;
		var circle = L.circle(point, {color:'#fff',fillOpacity:0,radius:i*m}).addTo(g_map);
		circle.bindPopup((i-1)*m+"~"+i*m+"m");
		circle.id = g_index++;
		g_layers.push(circle);
	}
}

//
function add_geo_center_line(data){
	var rsu_point = L.latLng(28.091180442,112.98596329);
	add_geo_rsu(rsu_point,"--","--");
	var count = 0;
    var features = data.features;
    for(var i in features){
        var feature = features[i];
        if(!feature.properties)continue;
        if(!feature.geometry)continue;
        if(feature.properties.type != "CenterLine")continue;
        if(feature.geometry.type != "LineString")continue;
        var latlngs = [];
        var coordinates = feature.geometry.coordinates;
        var id = feature.properties.parentLaneElId;
        for(var m in coordinates){
            var coor = coordinates[m];
            if(coor.length < 2)continue;
			// var dist = rsu_point.distanceTo(L.latLng(coor[1],coor[0]));
   //          if(dist > 500)continue;
            if(count++%3)continue;
			latlngs.push([coor[1],coor[0]]);
        }
        add_geo_lane_line(latlngs,id);
    }
}

//geo json
function geo_json(filename){
    $.getJSON(filename,function(data){
        add_geo_center_line(data);
    }).error(function() {alert("parse json file error : "+filename);});
}

