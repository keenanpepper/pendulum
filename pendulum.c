#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <portaudio.h>
#include <gtk/gtk.h>

#define RATE (48000)

typedef struct {
	/* First the four user-controlled parameters. */
	double natural_frequency;
	double damping;
	double driving_amplitude;
	double driving_frequency;
	/* Now the three dynamic variables. */
	double angle;
	double velocity;
	double driving_phase;
	/* These are governed by the equations:
	d/dt(angle) = velocity
	d/dt(velocity) = - natural_frequency^2 * sin(angle)
	                 - damping * velocity
	                 + cos(angle) * driving_amplitude * sin(driving_phase)
	d/dt(driving_phase) = driving_frequency
	*/
	double ping_strength;
} oscillator;

int oscillate(const void *input, void *output, unsigned long frameCount,
              const PaStreamCallbackTimeInfo *timeInfo,
              PaStreamCallbackFlags statusFlags, void *userData) {
	int i;
	oscillator *o = (oscillator *) userData;

	for (i = 0; i < frameCount; ++i) {
		double accel;

		/* The sine of the angle is the best thing to output,
		   because it's periodic, automatically bounded,
		   and sensitive to the angle even when it's small. */
		((int16_t *) output)[i] = sin(o->angle) * INT16_MAX / 2;

		/* If you look closely, this is actually the "leapfrog method". */
		o->angle += o->velocity / RATE;
		if (o->angle > M_PI) o->angle -= 2*M_PI;
		if (o->angle < -M_PI) o->angle += 2*M_PI;

		accel = - o->natural_frequency * o->natural_frequency * sin(o->angle);
		accel -= o->damping * o->velocity;
		accel += cos(o->angle) * o->driving_amplitude * sin(o->driving_phase);
		o->velocity += accel / RATE;

		o->driving_phase += o->driving_frequency / RATE;
		if (o->driving_phase > M_PI) o->driving_phase -= 2*M_PI;
	}

//	fprintf(stderr, "Normalized energy: %f\n", o->velocity * o->velocity / (4 * o->natural_frequency * o->natural_frequency) + (1 - cos(o->angle)) / 2);

	return paContinue;
}

void update_natural_frequency(GtkAdjustment *adjustment, oscillator *o) {
	o->natural_frequency = 2*M_PI*pow(10, gtk_adjustment_get_value(adjustment));
}

void update_damping(GtkAdjustment *adjustment, oscillator *o) {
	o->damping = pow(10, gtk_adjustment_get_value(adjustment));
}

void update_driving_amplitude(GtkAdjustment *adjustment, oscillator *o) {
	o->driving_amplitude = pow(10, gtk_adjustment_get_value(adjustment));
}

void update_driving_frequency(GtkAdjustment *adjustment, oscillator *o) {
	o->driving_frequency = 2*M_PI*pow(10, gtk_adjustment_get_value(adjustment));
}

void update_ping_strength(GtkAdjustment *adjustment, oscillator *o) {
	o->ping_strength = pow(10, gtk_adjustment_get_value(adjustment));
}

void ping(GtkButton *button, oscillator *o) {
	o->velocity += o->ping_strength;
}

gchar *format_log(GtkScale *scale, gdouble value) {
	int precision = gtk_scale_get_digits(scale) - floor(value);
	if (precision < 0) precision = 0;
	return g_strdup_printf("%.*f", precision, pow(10, value));
}

void create_window(oscillator *o) {
	GtkWidget *window, *vbox;
	GtkObject *natural_adjustment;
	GtkObject *damping_adjustment;
	GtkObject *amplitude_adjustment;
	GtkObject *frequency_adjustment;
	GtkObject *ping_adjustment;
	GtkWidget *natural_label, *natural_hscale;
	GtkWidget *damping_label, *damping_hscale;
	GtkWidget *amplitude_label, *amplitude_hscale;
	GtkWidget *frequency_label, *frequency_hscale;
	GtkWidget *ping_label, *ping_hscale, *ping_button;

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size(GTK_WINDOW(window), 377, 233);
	gtk_window_set_title(GTK_WINDOW(window), "Driven pendulum simulator");

	vbox = gtk_vbox_new(TRUE, 5);

	natural_adjustment = gtk_adjustment_new(0, 0, log10(RATE/2), 0.001, 1, 0);
	damping_adjustment = gtk_adjustment_new(0, -2, 4, 0.001, 1, 0);
	amplitude_adjustment = gtk_adjustment_new(0, -2, 10, 0.001, 1, 0);
	frequency_adjustment = gtk_adjustment_new(0, 0, log10(RATE/2), 0.001, 1, 0);
	ping_adjustment = gtk_adjustment_new(0, 0, 6, 0.1, 1, 0);

	natural_hscale = gtk_hscale_new(GTK_ADJUSTMENT(natural_adjustment));
	damping_hscale = gtk_hscale_new(GTK_ADJUSTMENT(damping_adjustment));
	amplitude_hscale = gtk_hscale_new(GTK_ADJUSTMENT(amplitude_adjustment));
	frequency_hscale = gtk_hscale_new(GTK_ADJUSTMENT(frequency_adjustment));
	ping_hscale = gtk_hscale_new(GTK_ADJUSTMENT(ping_adjustment));

	ping_button = gtk_button_new_with_label("Ping");

	gtk_scale_set_digits(GTK_SCALE(natural_hscale), 3);
	gtk_scale_set_digits(GTK_SCALE(damping_hscale), 3);
	gtk_scale_set_digits(GTK_SCALE(amplitude_hscale), 3);
	gtk_scale_set_digits(GTK_SCALE(frequency_hscale), 3);
	gtk_scale_set_digits(GTK_SCALE(ping_hscale), 1);

	natural_label = gtk_label_new("Natural frequency (Hz)");
	damping_label = gtk_label_new("Damping coefficient");
	amplitude_label = gtk_label_new("Driving amplitude");
	frequency_label = gtk_label_new("Driving frequency (Hz)");
	ping_label = gtk_label_new("Ping strength");

	gtk_container_add(GTK_CONTAINER(vbox), natural_label);
	gtk_container_add(GTK_CONTAINER(vbox), natural_hscale);
	gtk_container_add(GTK_CONTAINER(vbox), damping_label);
	gtk_container_add(GTK_CONTAINER(vbox), damping_hscale);
	gtk_container_add(GTK_CONTAINER(vbox), amplitude_label);
	gtk_container_add(GTK_CONTAINER(vbox), amplitude_hscale);
	gtk_container_add(GTK_CONTAINER(vbox), frequency_label);
	gtk_container_add(GTK_CONTAINER(vbox), frequency_hscale);

	gtk_container_add(GTK_CONTAINER(vbox), ping_label);
	gtk_container_add(GTK_CONTAINER(vbox), ping_hscale);
	gtk_container_add(GTK_CONTAINER(vbox), ping_button);

	gtk_container_add(GTK_CONTAINER(window), vbox);

	g_signal_connect((gpointer) natural_adjustment, "value-changed",
                         G_CALLBACK(update_natural_frequency), o);
	g_signal_connect((gpointer) damping_adjustment, "value-changed",
                         G_CALLBACK(update_damping), o);
	g_signal_connect((gpointer) amplitude_adjustment, "value-changed",
                         G_CALLBACK(update_driving_amplitude), o);
	g_signal_connect((gpointer) frequency_adjustment, "value-changed",
                         G_CALLBACK(update_driving_frequency), o);
	g_signal_connect((gpointer) ping_adjustment, "value-changed",
                         G_CALLBACK(update_ping_strength), o);

	g_signal_connect((gpointer) natural_hscale, "format-value",
	                 G_CALLBACK(format_log), NULL);
	g_signal_connect((gpointer) damping_hscale, "format-value",
	                 G_CALLBACK(format_log), NULL);
	g_signal_connect((gpointer) amplitude_hscale, "format-value",
	                 G_CALLBACK(format_log), NULL);
	g_signal_connect((gpointer) frequency_hscale, "format-value",
	                 G_CALLBACK(format_log), NULL);
	g_signal_connect((gpointer) ping_hscale, "format-value",
	                 G_CALLBACK(format_log), NULL);

	g_signal_connect((gpointer) ping_button, "clicked",
	                 G_CALLBACK(ping), o);

	g_signal_connect((gpointer) window, "delete-event", gtk_main_quit, NULL);

	gtk_widget_show(natural_hscale);
	gtk_widget_show(damping_hscale);
	gtk_widget_show(amplitude_hscale);
	gtk_widget_show(frequency_hscale);
	gtk_widget_show(natural_label);
	gtk_widget_show(damping_label);
	gtk_widget_show(amplitude_label);
	gtk_widget_show(frequency_label);
	gtk_widget_show(ping_label);
	gtk_widget_show(ping_hscale);
	gtk_widget_show(ping_button);
	gtk_widget_show(vbox);
	gtk_widget_show(window);
}

void set_up_portaudio(oscillator *o) {
	PaStream *stream;
	PaError error;

	Pa_Initialize();

	error = Pa_OpenDefaultStream(&stream, 0, 1, paInt16, RATE,
	                             paFramesPerBufferUnspecified,
	                             oscillate, o);

	if (error < 0) {
		fprintf(stderr, "PortAudio returned error number %d\n", error);
		exit(1);
	}

	Pa_StartStream(stream);
}

int main(int argc, char **argv) {
	oscillator o = {1, 1, 1, 1, 0, 0, 0};

	gtk_init(&argc, &argv);

	create_window(&o);

	set_up_portaudio(&o);

	gtk_main();

	return 0;
}

