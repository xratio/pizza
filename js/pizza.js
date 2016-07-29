//jQuery(document).ready(function (){
  //window.alert("sometext");
  //alert(Drupal.settings.pizza.testvar);
//});

// The code in the following block we'll be run by Drupal on document ready event.
(function ($) {
	var pricee = 0;
	var delivery_pricee = 0;
	var quantityy = 1;
// Drupal behaviours are functions executed by Drupal automatically
// to attach various event handlers, like mouse clicks for form elements.
// See https://www.drupal.org/node/756722 for details.
Drupal.behaviors.pizza = {
  attach: function (context, settings) {
    $('#edit-pizza-name-2, #edit-pizza-name-3, #edit-pizza-name-6, #edit-pizza-name-7').click(function() {
      // Get pizza id from radio button value.
      var pid = $(this).val();
      // Gep price from Drupal settings.
      var price = Drupal.settings.pizza.price[pid];
	  pricee = price;
	  price = (parseInt(delivery_pricee) + parseInt(pricee)) * parseInt(quantityy);
      $('#price').html('Price: ' + price);
    });
	
	$('#edit-region').click(function() {
      // Get region id from select form.
      var rid = $(this).val();
      // Gep price from Drupal settings.
      var delivery_price = Drupal.settings.pizza.delivery[rid];
	  delivery_pricee = delivery_price;
	  delivery_price = (parseInt(delivery_pricee) + parseInt(pricee)) * parseInt(quantityy);
	  $('#price').html('Price: ' + delivery_price);
    });
	
	$('#edit-pizza-q').click(function() {
      // Get quantity.
      var qid = $(this).val();
      // Gep price from Drupal settings.
      var quantity = Drupal.settings.pizza.quantity[qid];
	  quantityy = quantity;
	  quantity = (parseInt(delivery_pricee) + parseInt(pricee)) * parseInt(quantityy);
	  $('#price').html('Price: ' + quantity);
    });
  }
}
})(jQuery);