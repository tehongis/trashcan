let camera, scene, renderer;
let cameraOrtho, sceneOrtho;
let controls;
var model = new THREE.Group();
var line = new THREE.Line();

var angle = Math.random() / 10;
var oldtime;

function drawShip() {


    init();
    animate();
}

function init() {        

        // Sprites
        const lightSprite = new THREE.TextureLoader().load( 'sprite.png' );
        const materialWhiteSprite = new THREE.SpriteMaterial( { map:lightSprite, color: 0xffffff } );
        const materialRedSprite = new THREE.SpriteMaterial( { map: lightSprite, color: 0xff0000 } );
        const materialGreenSprite = new THREE.SpriteMaterial( { map: lightSprite, color: 0x00ff00 } );
        const materialYellowSprite = new THREE.SpriteMaterial( { map: lightSprite, color: 0xffff00 } );


        var elem = document.getElementById("view");

        cameraOrtho = new THREE.OrthographicCamera( - elem.clientWidth / 2, elem.clientWidth / 2, elem.clientHeight / 2, - elem.clientHeight / 2, 1, 10 );
        cameraOrtho.position.set( 0, 0, 2 );
        sceneOrtho = new THREE.Scene();

        camera = new THREE.PerspectiveCamera(45, elem.clientWidth/elem.clientHeight, 1, 2000);
        camera.position.set( 0, -1.5, 12 );
        scene = new THREE.Scene();
        scene.add(new THREE.HemisphereLight(0x4f4f4f, 0.5));
        //scene.add(new THREE.AmbientLight(0x1a1a3a, 0.15));
        scene.fog = new THREE.Fog( 0x000000, 1, 80 );
      
        const materialBoat = new THREE.MeshPhongMaterial({ 
            color: 0xf0f0ff,
            specular: 100, 
            shininess: 10,
            reflectivity: 0.2 });



        // Boat model
        const loader = new THREE.STLLoader()
        loader.load("sailboat.stl", function (geometry) {
            mesh = new THREE.Mesh(geometry, materialBoat)
            var middle = new THREE.Vector3();
            geometry.computeBoundingBox();
            geometry.boundingBox.getCenter(middle);   
            mesh.geometry.applyMatrix4(new THREE.Matrix4().makeTranslation( -middle.x, -middle.y, -middle.z ) );
            mesh.castShadow = true
            mesh.receiveShadow = true  
            geometry.center()
            model.add(mesh);
        })
        //model.rotateY(THREE.MathUtils.degToRad(angle));
//        document.getElementById("angle").innerHTML =  angle;
        
        // Plane
        const planeGeometry = new THREE.PlaneGeometry(2000, 2000);
        const materialPlane = new THREE.MeshPhongMaterial({ 
                                color: 0x2f2f3f,
                                specular: 20, 
                                shininess: 90,
                                reflectivity: 0.9 }); 
        const plane = new THREE.Mesh( planeGeometry, materialPlane );
        plane.rotateX(-Math.PI / 2);
        plane.receiveShadow = true;
        plane.position.set(0, -2.3, 0);
        scene.add(plane);

        // renderer
        renderer = new THREE.WebGLRenderer({ antialias: false, alpha: false });
        renderer.setSize(elem.clientWidth, elem.clientHeight);
        renderer.autoClear = false; // To allow render overlay on top of sprited sphere
        elem.appendChild(renderer.domElement);
        //document.body.appendChild( renderer.domElement );

        
/*        
        controls = new THREE.OrbitControls(model, renderer.domElement);
        controls.enableDamping = true;
        controls.rotateSpeed = 0.05;
        controls.dampingFactor = 0.1;
        controls.enableZoom = true;
        controls.autoRotate = true;
        controls.autoRotateSpeed = .75;
*/        

        //const spriteMastRed = new THREE.Sprite( materialRedSprite );
        //spriteMastRed.position.set(0, 2.7, 0);
        //spriteMastRed.scale.set(.4,.4,.4);  // imageWidth, imageHeight
        //mesh.add( spriteMastRed );

/*
        group = new THREE.Group();
        group.add( sprite );
        scene.add( group );
*/

        const spriteMastWhite = new THREE.Sprite( materialWhiteSprite );
        spriteMastWhite.position.set(0, 2.7, 0);
        spriteMastWhite.scale.set(.4,.4,.4);  // imageWidth, imageHeight
        if (angle >= 65.5 && angle <= 252.5 ) {
            model.add( spriteMastWhite );
        }

        const spriteRear = new THREE.Sprite( materialWhiteSprite );
        spriteRear.position.set(0, -1.7, 2);
        spriteRear.scale.set(.4,.4,.4);  // imageWidth, imageHeight
        if (angle >= 252.5 || angle <= 67.5 ) {
            model.add( spriteRear );
        }

        const spritePortside = new THREE.Sprite( materialRedSprite );
        spritePortside.position.set(-1, -1.7, 0);
        spritePortside.scale.set(.4,.4,.4);  // imageWidth, imageHeight
        if (angle >= 65.5 && angle <= 180.0 ) {
            model.add( spritePortside );
        }
    

        const spriteStarboard = new THREE.Sprite( materialGreenSprite );
        spriteStarboard.position.set(1, -1.7, 0);
        spriteStarboard.scale.set(.4,.4,.4);  // imageWidth, imageHeight
        if (angle >= 180 && angle <= 242.5 ) {
            model.add( spriteStarboard );
        }

        scene.add(model);

        // compass
        const geometryCircle = new THREE.CircleGeometry( 80, 32 );
        const materialCircle = new THREE.MeshBasicMaterial( { color: 0x3f3f3f } );
        const circle = new THREE.Mesh( geometryCircle, materialCircle );
        circle.position.set(-220, 150, 0);
        sceneOrtho.add( circle );

        const materialLine = new THREE.LineBasicMaterial( { color: 0xffffff } );
        const points = [];
        points.push( new THREE.Vector3( 0, 0, 0 ) );
        points.push( new THREE.Vector3( 0, 75, 0 ) );
        points.push( new THREE.Vector3( 5, 20, 0 ) );
        points.push( new THREE.Vector3( -5, 20, 0 ) );
        points.push( new THREE.Vector3( 0, 75, 0 ) );
        const geometryLine = new THREE.BufferGeometry().setFromPoints( points );
        line = new THREE.Line( geometryLine, materialLine );
        //line.rotateZ(THREE.MathUtils.degToRad(angle));
        line.position.set( -220, 150, 0);
        sceneOrtho.add( line );
    
}

function animate() {

    requestAnimationFrame( animate );
    render();

}

function onWindowResize() {
    
    renderer.setSize(elem.clientWidth, elem.clientHeight);

}


function render() {
    const time = Date.now() / 1000;
    var timediff = time - oldtime;
    
//    controls.update();
    var boatAngle = THREE.MathUtils.radToDeg(model.rotation.y);
    document.getElementById("angle").innerHTML =  boatAngle.toPrecision(4);
    model.rotateY(THREE.MathUtils.degToRad(angle));
    line.rotateZ(THREE.MathUtils.degToRad(angle));  

    renderer.clear();
    renderer.render( scene, camera );
    renderer.clearDepth();
    renderer.render( sceneOrtho, cameraOrtho );
    oldtime=time;
}
