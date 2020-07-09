var spd = 1;
var cenX = 0;
var oldX = 0;
var oldY = 0;
var posX = 0;
var posY = 0;
var ufoX = 0;
var _ufoX = 0;
var maxX;
var maxY;
var pew = new Array();
document.onkeydown = checkKey;
function checkKey(e) {
	e = e || window.event;
	switch (e.keyCode) {
	case 17:
		pew.push([cenX, posY - spd]);
		break;
	case 37:
		posX -= spd;
		break;
	case 38:
		posY -= spd;
		break;
	case 39:
		posX += spd;
		break;
	case 40:
		posY += spd;
		break;
	default:
		alert("key " + e.keyCode);
		break;
	};
};
function pewRender(ctx, img) {
	for (i in pew) {
		ctx.clearRect(pew[i][0] - 1, pew[i][1] + spd, spd + 1, spd + 1);
		if (pew[i][1] < img.height)
			if (pew[i][0] > ufoX)
				if (pew[i][0] < ufoX + img.width) {
					ctx.drawImage(img, ufoX, 0);
					ufoX = ctx.lineWidth;
					delete pew[i];
				};
	};
	if (posY < img.height)
		if (cenX > ufoX)
			if (cenX < ufoX + img.width) {
				ctx.drawImage(img, ufoX, 0);
				ufoX = ctx.lineWidth;
			};
	for (i in pew) {
		if ("object" != typeof(pew[i]))
			continue;
		if (pew[i][1] > ctx.lineWidth) {
			ctx.fillStyle = "#FF0000";
			ctx.fillRect(pew[i][0], pew[i][1], 2, 2);
		};
		pew[i][1] -= spd;
	};
	var _pew = new Array();
	for (i in pew) {
		if ("object" != typeof(pew[i]))
			continue;
		if (pew[i][1] > ctx.lineWidth - spd)
			_pew.push(pew[i]);
	};
	pew = _pew;
}
function ufoRender(ctx, img) {
	if (ufoX > ctx.canvas.width - img.width - ctx.lineWidth)
		ufoX = ctx.lineWidth;
	ctx.clearRect(_ufoX, 0, img.width + spd, img.height);
	ctx.drawImage(img, ufoX, 0);
	_ufoX = ufoX;
	ufoX += spd;
}
function imgRender(ctx, img) {
	if (posX > maxX)
		posX = maxX;
	if (posX < ctx.lineWidth)
		posX = ctx.lineWidth;
	if (posY > maxY)
		posY = maxY;
	if (posY < ctx.lineWidth)
		posY = ctx.lineWidth;
	if (oldX != posX || oldY != posY) {
		if (oldX > 0)
			if (oldY > 0)
				ctx.clearRect(oldX, oldY, img.width, img.height);
		ctx.drawImage(img, posX, posY);
		cenX = posX + img.width / 2;
	};
	oldX = posX;
	oldY = posY;
};
function getMousePos(canvas, evt) {
	var rect = canvas.getBoundingClientRect();
	pew.push([cenX, posY - spd]);
	return {
		x: evt.clientX - rect.left,
		y: evt.clientY - rect.top
	};
};
function process() {
	var textCanvas = document.getElementById("text");
	var ctx = textCanvas.getContext("2d");
	ctx.canvas.addEventListener("click", function (evt) {
		var mousePos = getMousePos(textCanvas, evt);
		posX = mousePos.x;
		posY = mousePos.y;
	}, false);
	ctx.clearRect(0, 0, ctx.canvas.width, ctx.canvas.height);
	ctx.fillStyle = "#333333";
	ctx.textAlign = "center";
	ctx.textBaseline = "middle";
	ctx.font = "12px monospace";
	ctx.globalCompositeOperation = "destination-over";
	ctx.fillStyle = "#FFFFFF";
	ctx.fillRect(0, 0, ctx.canvas.width, ctx.canvas.height);
	ctx.globalCompositeOperation = "source-over";
	ctx.lineWidth = 2;
	ctx.strokeStyle = "#000000";
	ctx.strokeRect(0, 0, ctx.canvas.width, ctx.canvas.height);
	var buran = new Image();
	buran.src = "img/buran72.png";
	spd = buran.width / 10;
	posX = (ctx.canvas.width - buran.width - ctx.lineWidth) / 2;
	posY = (ctx.canvas.height - buran.height - ctx.lineWidth) / 2;
	maxX = ctx.canvas.width - buran.width - ctx.lineWidth;
	maxY = ctx.canvas.height - buran.height - ctx.lineWidth;
	var imgTimer = setInterval(imgRender, 100, ctx, buran);
	var ufo = new Image();
	ufo.src = "img/ufo01.png";
	var ufoTimer = setInterval(ufoRender, 100, ctx, ufo);
	var _ufo = new Image();
	_ufo.src = "img/ufo01_1.png";
	var pewTimer = setInterval(pewRender, 100, ctx, _ufo);
	ctx.clearRect(0, 0, ctx.canvas.width, ctx.canvas.height);
};
