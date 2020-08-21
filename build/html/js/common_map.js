

// 地图类型
var mapType = {
    localGoogleSatellite  : 0,
    remoteGoogleSatellite : 1,
    localGoogleStreet     : 2,
    remoteGoogleStreet    : 3
}


// 全局变量
var g_arg = {
    obj     : null,
    map     : null,
    street  : null,
    type    : mapType.localGoogleSatellite,
    center  : [28.1128547,112.8668242],
    zoom    : 17,
    minZoom : 3,
    maxZoom : 22,
    group   : null
}

var g_lng;
var g_lat;

// 初始化地图
function mapInit(){
    var url = "map/s/{z}/{x}/{y}.jpg";
//    g_arg.type = mapType.remoteGoogleStreet;
    if(g_arg.type == mapType.localGoogleSatellite){
        url = "map/s/{z}/{x}/{y}.jpg";
    }else if(g_arg.type == mapType.localGoogleStreet){
        url = "map/m/{z}/{x}/{y}.jpg";
    }else if(g_arg.type == mapType.remoteGoogleSatellite){
        url = "http://{s}.google.com/vt/lyrs=s&x={x}&y={y}&z={z}";
    }else if(g_arg.type == mapType.remoteGoogleStreet){
        url = "http://{s}.google.com/vt/lyrs=m&x={x}&y={y}&z={z}";
    }
    
    if(g_arg.map == null){
        g_arg.map = L.map('googleMap',{zoomAnimation:true});
        g_arg.map.setView(g_arg.center,g_arg.zoom);
        if(g_arg.obj != null){
            g_arg.map.on("move",function(event){
                var b = g_arg.map.getBounds();
                var zoom = g_arg.map.getZoom();
                g_arg.obj.getBounds(b.getSouth(),b.getNorth(),b.getWest(),b.getEast(),zoom,g_arg.type);
            })
        }
    }
    
    if(g_arg.map.hasLayer(g_arg.street))g_arg.map.removeLayer(g_arg.street);
    g_arg.street = L.tileLayer(url,{ minZoom: g_arg.minZoom,maxZoom: g_arg.maxZoom,subdomains:['mt0','mt1','mt2','mt3']});
    g_arg.street.addTo(g_arg.map);
	if(g_arg.group == null){
		g_arg.group = L.markerClusterGroup();
		g_arg.map.addLayer(g_arg.group);
	}
}

// 在地图中心添加marker
function addMarkerAtCenter(){
    var center = g_arg.map.getCenter();
    addMarker(center.lng,center.lat);
}

// 添加marker
function addMarker(lng,lat){
    lat = lat.toFixed(7);
    lng = lng.toFixed(7);
    var marker = L.marker([lat,lng]).addTo(g_arg.map);
	marker.dragging.enable();
    marker.bindPopup("lng : "+lng+"</br>lat : "+lat);
    marker.on('dragend',function(event){
        var pt = marker.getLatLng();
        var lat_new = pt.lat.toFixed(7);
        var lng_new = pt.lng.toFixed(7);
        marker.setPopupContent("lng : "+lng_new+"</br>lat : "+lat_new);
	});
    g_arg.map.panTo([lat,lng],{"animate":true,"duration":1});
}



// -----------------------------------map json 解析--------------------------------------------------

// 添加车道的折线
function addLaneLine(latlngs,id){
    var len = latlngs.length ,i,angle = 0;
    var line_color = "#FF0000";
    if(len == 0 )return;
    for(i=0;i<len;i++){
        if(i<len-1)angle = Math.atan2(latlngs[i+1][1]-latlngs[i][1],latlngs[i+1][0]-latlngs[i][0])*180/Math.PI;
        addNodePoint(latlngs[i],angle);
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
function addNodePoint(latlng,angle)
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
function addCenterPoint(lat,lng,n_id,n_region){
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
            addLaneLine(latlngs,lanes[j].laneID);
        }
    }
}

// map
function addMapJson(data)
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
        addCenterPoint(lat,lng,node_id,node_region);
        g_map.panTo(center,{animate:true,duration:0.5});
        json_links_parse(inlinks);
    }
}

// rsi
function addRsiJson(data)
{
    g_lng = data.refPos.lng;
    g_lat = data.refPos.lat;
    var lng = (g_lng*1e-7).toFixed(7);
    var lat = (g_lat*1e-7).toFixed(7);
    var center = [lat,lng];
    g_map.panTo(center,{animate:true,duration:0.5});
    addCenterPoint(lat,lng,"--","--");
    var latlngs = [];
    for(var k in data.alertPath){
        var p = data.alertPath[k];
        var plng = (p.lng*1e-7).toFixed(7);
        var plat = (p.lat*1e-7).toFixed(7);
        latlngs.push([plat,plng]);
    }
    addLaneLine(latlngs,"--");
}

// 解析json文件
function mapParse(filename){
	$.getJSON(filename,function(data){
        var type = "map";
        if(data.type)type = data.type;
        if(type == "map")addMapJson(data);
        if(type == "rsi")addRsiJson(data);
	}).error(function() {alert("解析json失败 , "+filename);});
}


