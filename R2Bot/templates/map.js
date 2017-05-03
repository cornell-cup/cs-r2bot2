function zeros(dimensions) {
    var array = [];

    for (var i = 0; i < dimensions[0]; ++i) {
        array.push(dimensions.length == 1 ? 0 : zeros(dimensions.slice(1)));
    }

    array[15][15] = 1;

    return array;
}

var mctx = document.getElementById('main').getContext('2d');

function arrayBufferToString(buffer) {
    var arr = new Uint8Array(buffer);
    var str = String.fromCharCode.apply(String, arr);
    if (/[\u0080-\uffff]/.test(str)) {
        throw new Error("this string seems to contain (still encoded) multibytes");
    }
    return str;
}

function drawMap(data) {
    var map = zeros([30, 30]);

    var dist = arrayBufferToString(data);

    console.log(dist);

    //splitting up array of sensor data and creating dictionary of sensor:sensorData
    var n = dist.split("\n");
    console.log(n);

    var dict = {};
    for (var i = 0; i < n.length; i++) {
        var split = n[i].split(',');
        //   var subs = split[0].substring(1);
        // subs = subs.substring(0, subs.length - 1);
        if (parseFloat(split[1]) <= 20.0) {
            dict[split[0]] = parseFloat(split[1]) / 2;
        }
        console.log(dict);
    }
    //storing values in sensor and distance 
    for (var key in dict) {
        var sensor = key;
        console.log(sensor);
        var distance = dict[key];
        console.log(getAngle(sensor));
        angle = parseFloat(getAngle(sensor)) * Math.PI / 180;
        console.log("angle" + angle);
        findEdge(distance, angle, sensor);
    }

    for (var i = 0; i < map.length; i++) {
        for (var j = 0; j < map[i].length; j++) {
            if (map[i][j] === 1) {
                console.log("this is red");
                mctx.fillStyle = 'Red';
                mctx.fillRect(j * 10, i * 10, 10, 10);
                //  map[i][j + dist] = 2;
            }
            if (map[i][j] == 2) {
                console.log("this is yellow");
                mctx.fillStyle = 'Yellow';
                mctx.fillRect(j * 10, i * 10, 10, 10);
            }
            if (map[i][j] == 3) {
                console.log("this is cyan");
                mctx.fillStyle = 'Cyan';
                mctx.fillRect(j * 10, i * 10, 10, 10);
            }
            if (map[i][j] == 4) {
                console.log("this is green");
                mctx.fillStyle = 'Green';
                mctx.fillRect(j * 10, i * 10, 10, 10);
            }

        }
    }

    function findEdge(distance, angle, sensor) {
        //R2 position = map[15][15]

        var centerAngle = angle;
        console.log("ca" + centerAngle);
        for (var i = 0; i < 4; i++) { //going counterclockwise
            var xComponent = Math.floor(distance * Math.cos(centerAngle));
            console.log(distance * Math.cos(centerAngle));
            var yComponent = Math.floor(distance * Math.sin(centerAngle));
            console.log("going ccw");
            // console.log(xComponent);
            console.log(yComponent);

            map[15 - yComponent][15 + xComponent] = getColor(sensor);
            centerAngle += 5 * Math.PI / 180;
        }

        for (var i = 0; i < 4; i++) { //going clockwise
            var xComponent = Math.floor(distance * Math.cos(angle));
            var yComponent = Math.floor(distance * Math.sin(angle));
            console.log("going cw");
            console.log(xComponent);
            console.log(yComponent);
            map[15 - yComponent][15 + xComponent] = getColor(sensor);
            angle -= 5 * Math.PI / 180;
        }
        //column, row
    }
    function getColor(sensor) {
        var color = 0;
        switch (sensor) {
            case "U1SENSOR":
                color = 2;
                break;
            case "U2SENSOR":
                color = 3;
                break;
            case "U3SENSOR":
                color = 4;
                break;
        }

        return color;
    }
    function getAngle(sensor) {
        var angle = 0;
        switch (sensor) {
            case "U1SENSOR":
                angle = 90;
                break;
            case "U2SENSOR":
                angle = 67.5;
                break;
            case "U3SENSOR":
                angle = 112.5;
                break;
        }

        return angle;
    }
}

function drawBoard() {
    for (var x = 0; x <= 301; x += 10) {
        mctx.moveTo(0.5 + x + 0, 0);
        mctx.lineTo(0.5 + x + 0, 300 + 0);
    }

    for (var x = 0; x <= 301; x += 30) {
        ctx.moveTo(0, 0.5 + x + 0);
        ctx.lineTo(300 + 0, 0.5 + x + 0);
    }
    for (var x = 0; x <= 301; x += 10) {
        mctx.moveTo(0, 0.5 + x + 0);
        mctx.lineTo(300 + 0, 0.5 + x + 0);
    }

    mctx.strokeStyle = "black";
    mctx.stroke();

}